#include <imgui.h>
#include <ImGuizmo.h>
#include <backends/imgui_impl_sdl.h>

#include "engine.hpp"
#include "renderer/graph.hpp"
#include "editor/editor_render.hpp"

using namespace Editor;

bool EditorRender::editingDrag = false;
bool EditorRender::editingText = false;
Vector2 EditorRender::mouseLockPosition {};
Vector2 EditorRender::gameWindowSize {};

inline void CheckEngineQuitEvent()
{
    if (Game::Inputs::quit)
    {
        ImGui::OpenPopup("QUIT");
        Game::Inputs::quit = false;
    }

    /// CREATE NEW SCENE POPUP
    if (ImGui::BeginPopupModal("QUIT", nullptr))
    {
        ImGui::Text("Are you sure to close the engine?\nDon't forget to save your current scene!\n");

        ImGui::Separator();

        if (ImGui::Button("Quit", ImVec2(120, 0)))
        {
            Game::Inputs::quit = true;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(120, 0)))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }
}

EditorRender::EditorRender(Engine& io_engine)
{
    ImGuiIO& io = ImGui::GetIO();

    io.Fonts->AddFontFromFileTTF("editor/assets/Louis_George_Cafe_Bold.ttf", 15.f);

    menuBar.FrakkasColors();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 6.0f;
        style.ChildRounding = 6.0f;
        style.FrameRounding = 3.0f;
        style.GrabRounding = 12.0f;
        style.TabRounding = 6.0f;
    }

    // BIND FUNCTION TO ENGINE
    UpdateEvent editorUpdateEvent = [&](){ UpdateAndRender(io_engine);};
    io_engine.updateEventsHandler.emplace_back(editorUpdateEvent);
    UpdateEvent editorCameraUpdateEvent = [&]()
    {
        if (!scene.isMoving) return;

        Game::Transform& camTrs = io_engine.GetEditorCameraTransform();
        float frameSpeed = 20.f * Game::Time::GetRawDeltaTime();

        float forwardVelocity = Game::Inputs::GetAxis("forward");
        float strafeVelocity = Game::Inputs::GetAxis("horizontal");
        float verticalVelocity = Game::Inputs::GetAxis("verticalDebug");

        Vector3 rot = camTrs.rotation;

        float invertY = Maths::Abs(rot.x) > Maths::Constants::halfPi ? -1.f : 1.f;
        static float hold = 0.5f;

        Vector3 dir = Vector3::zero;

        dir.x += Maths::Sin(rot.y) * invertY * forwardVelocity + Maths::Cos(rot.y) * strafeVelocity;
        dir.y += Maths::Sin(rot.x) * forwardVelocity;
        dir.z += Maths::Sin(rot.y) * strafeVelocity - Maths::Cos(rot.y) * invertY * forwardVelocity;

        dir.x -= Maths::Sin(rot.x) * verticalVelocity * Maths::Sin(rot.y);
        dir.y += Maths::Cos(rot.x) * verticalVelocity;
        dir.z += Maths::Sin(rot.x) * verticalVelocity * Maths::Cos(rot.y);

        dir = dir.Normalize();
        if (dir.Length() > 0.f)
            hold += Game::Time::GetRawDeltaTime();
        else
            hold = 0.5f;

        rot.y += Game::Inputs::GetMouseDelta().x * 0.005f;
        rot.x -= Game::Inputs::GetMouseDelta().y * 0.005f;

        rot.x = Maths::Abs(rot.x) > Maths::Constants::pi + Maths::Constants::weakEpsilon ? -rot.x : rot.x;
        rot.y = Maths::Modulo(rot.y, Maths::Constants::doublePi);

        camTrs.position = camTrs.position.get() + dir * frameSpeed * hold * hold;
        camTrs.rotation = rot;
    };

    io_engine.updateEventsHandler.emplace_back(editorCameraUpdateEvent);

    Renderer::OnSceneLoadEvent onSceneLoadEvent = [&]()
    {
        hierarchy.selected = nullptr;
    };

    io_engine.graph->sceneLoadEvents.emplace_back(onSceneLoadEvent);
}

void EditorRender::UpdateAndRender(Engine& io_engine)
{
    ImGuizmo::BeginFrame();

    bool stylePushed = false;
    if (io_engine.GetRunMode() & Utils::UpdateFlag_Gaming)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, io_engine.GetRunMode() & Utils::UpdateFlag_Editing ? 0.75f : 0.4f);

        stylePushed = true;

        if (!(io_engine.GetRunMode() & Utils::UpdateFlag_Editing))
            hierarchy.selected = nullptr;
    }

    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    // Reset drag edit state
    if (editingDrag && Game::Inputs::IsReleased(Game::EButton::MOUSE_LEFT))
    {
        Engine::SetCursorGameMode(false);
        Engine::SetCursorPosition(mouseLockPosition);
        editingDrag = false;
    }

    // Unfocus whatever item with ESCAPE
    if (Game::Inputs::IsPressed(Game::EButton::ESCAPE))
        ImGui::SetWindowFocus(nullptr);

    // Reset editing text if needed
    if(!ImGui::IsAnyItemActive())
        Editor::EditorRender::editingText = false;

    // Edit entities with inputs
    hierarchy.OnImGuiRender(io_engine.entityManager);
    Inspector::OnImGuiRender(hierarchy.selected, guizmoOperation);

    // Disable inputs if typing
    if (editingText || !(io_engine.GetRunMode() & Utils::UpdateFlag_Editing) || scene.isMoving)
        io_engine.DisableInputs();

    // reload scene if menu bar call scene reloading
    bool reloadScene = false;
    MenuBar::OnImGuiRender(io_engine, reloadScene, hierarchy.selected);
    if (reloadScene)
        io_engine.SetRunMode(Utils::UpdateFlag_Editing);

    // Render viewers
    debugger.OnImGuiRender();
    resources.OnImGuiRender();
    console.OnImGuiRender();
    fileBrowser.OnImGuiRender();

    // Render scenes
    if (scene.isMoving)
        io_engine.EnableInputs();

    scene.OnImGuiRender(io_engine, hierarchy.selected, guizmoOperation);

    if(stylePushed)
        ImGui::PopStyleVar();

    io_engine.EnableInputs();
    game.OnImGuiRender(io_engine);

    CheckEngineQuitEvent();

    io_engine.gameFBO->size = {gameWindowSize.x - ImGuiWindowPadding, gameWindowSize.y - ImGuiWindowPadding};
}
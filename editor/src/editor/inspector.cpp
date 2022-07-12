#include <imgui.h>

#include "game/entity.hpp"

#include "helpers/game_edit.hpp"
#include "helpers/drop_component.hpp"

#include "editor/inspector.hpp"

using namespace Editor;

void Inspector::OnImGuiRender(Game::Entity* io_selectedEntity, ImGuizmo::OPERATION& i_guizmoOperation)
{
    ImGui::Begin("Inspector");

    if (io_selectedEntity)
    {
        Helpers::Edit(*io_selectedEntity, i_guizmoOperation);
        DragDropTargetButton(io_selectedEntity);
        AddComponentPopup(io_selectedEntity);
    }

    ImGui::End();
}

void Editor::Inspector::DragDropTargetButton(Game::Entity* io_selectedEntity)
{
    ImGui::InvisibleButton("InvisibleDragDropButton", ImVec2(500, 500));

    Helpers::DragDropTargetComponent(*io_selectedEntity);
}

void Inspector::AddComponentPopup(Game::Entity* io_selectedEntity)
{
    /// ADD NEW COMPONENT POPUP
    ImGui::SetNextWindowSize(ImVec2(0, 200));
    if (ImGui::BeginPopup("Add component.", ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_AlwaysVerticalScrollbar))
    {
        static ImGuiTextFilter filter;

        // Set keyboard focus on filter only when window popup
        static bool firstTime = true;
        if (firstTime) ImGui::SetKeyboardFocusHere();
        firstTime = false;

        filter.Draw("Component", 180);

        for (const ClassMetaData* metaData : Game::Component::GetRegistry())
        {
            std::string name = metaData->className;
            if (!metaData->publicClass || !filter.PassFilter(name.c_str()))
                continue;

            if (ImGui::Selectable(name.c_str()))
            {
                io_selectedEntity->AddComponent(std::unique_ptr<Game::Component>(metaData->constructor()));
                firstTime = true;
            }
        }

        ImGui::EndPopup();
    }
}
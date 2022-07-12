#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl.h>

#include <glad/glad.h>
#include <SDL.h>
#include <Tracy.hpp>

#include "maths.hpp"
#include "debug/log.hpp"
#include "game/entity.hpp"
#include "camera.hpp"
#include "game/world.hpp"

#include "ui/canvas.hpp"

#include "multithread/threadpool.hpp"

#include "resources/resources_manager.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/graph.hpp"

#include "engine.hpp"

#ifdef KK_WINDOWS
// Run on laptop high perf GPU
extern "C"
{
    __declspec(dllexport) int NvOptimusEnablement = 1;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

SDL_Window* Engine::window = nullptr;
ma_engine Engine::soundEngine {};

inline Vector2 AdaptSize(Vector2& size, float aspectRatio)
{
    Vector2 oldSize = size, offset;
    float newAspect = size.x / size.y;
    if (newAspect > aspectRatio)
        size.x = aspectRatio * size.y;
    else if (newAspect < aspectRatio)
        size.y = size.x / aspectRatio;

    offset.x = (oldSize.x - size.x) / 2.f;
    offset.y = (oldSize.y - size.y) / 2.f;

    return offset;
}

Engine::Engine()
{
    Log::Init();

    InitSDL();
    InitImGui();
    InitMiniaudio();

    renderer = std::make_unique<Renderer::LowLevel::LowRenderer>();
    editorFBO = std::make_unique<Renderer::LowLevel::Framebuffer>(1920, 1080, Renderer::LowLevel::Framebuffer::ERenderMode::FREE_ASPECT);
    gameFBO = std::make_unique<Renderer::LowLevel::Framebuffer>(1920, 1080, Renderer::LowLevel::Framebuffer::ERenderMode::LOCK_ASPECT);

    graph = std::make_unique<Renderer::Graph>(&entityManager, &physicScene, renderer.get());

    gameWorld = std::make_unique<Game::World>(*this);

    SetUINavigation(true);
}

Engine::~Engine()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    graph->Clear();

    ma_engine_uninit(&soundEngine);
}

static void DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
        return;
    // Ignore issues that we can't fix
    if (std::string(message).find("being recompiled based on GL state"))
        return;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            Log::Error("[HIGH] OPENGL: " + std::string(message));
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            Log::Warning("[MEDIUM] OPENGL: " + std::string(message));
            break;
        case GL_DEBUG_SEVERITY_LOW:
            Log::Warning("[LOW] OPENGL: " + std::string(message));
            break;
        default:
            Log::Warning("[UNKNOWN] OPENGL: " + std::string(message));
    }
}

void Engine::InitSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0)
    {
        Log::Error("SDL: Can't init SDL - " + static_cast<std::string>(SDL_GetError()));
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = static_cast<SDL_WindowFlags>(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    window = SDL_CreateWindow("Frakkas", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);

    if (!window)
    {
        Log::Error("SDL: Can't create window - " + static_cast<std::string>(SDL_GetError()));
        exit(1);
    }

    glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);
    SDL_GL_SetSwapInterval(1);

    // Load GL extensions using glad
    if (!gladLoadGLLoader(static_cast<GLADloadproc>(SDL_GL_GetProcAddress)))
    {
        Log::Error("OPENGL: Failed to initialize the OpenGL context");
        exit(1);
    }

    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(DebugCallback, nullptr);
    }
}

void Engine::InitImGui()
{
// Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.BackendFlags |= ImGuiBackendFlags_HasMouseHoveredViewport;

    ImGui_ImplSDL2_InitForOpenGL(SDL_GL_GetCurrentWindow(), SDL_GL_GetCurrentContext());
    ImGui_ImplOpenGL3_Init("#version 450");

    // BIND FUNCTION TO ENGINE

    editorInputsEvent = [](const SDL_Event* event) { return ImGui_ImplSDL2_ProcessEvent(event); };
}

void Engine::InitMiniaudio()
{
    if (ma_engine_init(nullptr, &soundEngine) != MA_SUCCESS)
    {
        Log::Error("Failed to initialize audio engine.");
        exit(1);
    }

    ma_engine_start(&soundEngine);
    Log::Info("Successfully start sound engine");
}

void Engine::BeginFrame()
{
    ThreadPool::PollMainThreadTasks();

    inputsManager.PollEvent(editorInputsEvent);
    Game::Time::NewFrame();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

bool Engine::EndFrame()
{
    SDL_GL_SwapWindow(window);
    FrameMark

    graph->ProcessLoading();

    bool quit = Game::Inputs::quit;
    if (quit)
        ResourcesManager::DestroyResources();
    return  !quit;
}

void Engine::RenderImGui() const
{
    ImGuiIO& io = ImGui::GetIO();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
}

void Engine::RunEditor()
{
    bool running = true;
    while(running)
    {
        BeginFrame();
        
        /// OnUpdate
        if (updateMode & Utils::UpdateFlag_Gaming)
        {
            // Disable inputs for game update if editing
            if (updateMode & Utils::UpdateFlag_Editing)
                DisableInputs();

            if (!graph->loading)
                entityManager.Update();
        }

        EnableInputs();
        for (const UpdateEvent& updateEvent : updateEventsHandler)
            updateEvent();

        if (!graph->loading)
            physicScene.Update(updateMode);

        /// Draw
        graph->RenderEditor(*renderer, editorFBO->AspectRatio());
        renderer->RenderFinalScreen(*editorFBO);

        graph->RenderGame(*renderer, gameFBO->AspectRatio());
        renderer->RenderFinalScreen(*gameFBO);

        glViewport(0, 0, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        RenderImGui();

		running = EndFrame();
    }
}

void Engine::RunGame()
{
    EnableInputs();
    SetRunMode(Utils::UpdateFlag_Gaming);

    int width = 0, height = 0;
    Vector2 windowSize(width, height);

    gameFBO->lockAspectRatio = true;

    bool running = true;
    while(running)
    {
        BeginFrame();

        SDL_GetWindowSize(window, &width, &height);
        windowSize = {static_cast<float>(width), static_cast<float>(height)};

        if (!graph->loading)
        {
		    entityManager.Update();

            for (const UpdateEvent& updateEvent : updateEventsHandler)
                updateEvent();

            physicScene.Update();
        }

        renderer->SetWindowSize(windowSize);

        graph->RenderGame(*renderer, renderer->firstPassFBO->AspectRatio());
        glViewport(renderer->firstPassFBO->offset.x, renderer->firstPassFBO->offset.y, windowSize.x, windowSize.y);
        renderer->RenderFinalScreen();

        if (graph->loading || !graph->loadingCanvas.FadeAway())
            graph->loadingCanvas.BeginAndRender();
        else
            graph->canvas.BeginAndRender();

        RenderImGui();

        /// ENDFRAME
        running = EndFrame();
    }
}

void Engine::SetRunMode(unsigned int i_flag)
{
    updateMode = i_flag;
    graph->playing = updateMode & Utils::UpdateFlag_Gaming;

    if (updateMode & Utils::UpdateFlag_Gaming
    && !(Game::World::GetInputsMode() & Game::World::InputsMode_UI))
    {
        SetUINavigation(false);
        SetCursorGameMode(true);
    }

    if (updateMode & Utils::UpdateFlag_Editing)
    {
        SetUINavigation(true);
        SetCursorGameMode(false);
    }

    graph->playing = updateMode & Utils::UpdateFlag_Gaming;
}

Utils::UpdateFlag Engine::GetRunMode() const
{
    return updateMode;
}

void Engine::SetCursorVisibility(bool i_visibility)
{
    SDL_ShowCursor(i_visibility ? SDL_ENABLE : SDL_DISABLE);
}

void Engine::SetCursorGameMode(bool i_gameMode)
{
    SDL_SetRelativeMouseMode(static_cast<SDL_bool>(i_gameMode));
}

void Engine::SetCursorPosition(const Vector2& i_position)
{
    SDL_WarpMouseInWindow(window, i_position.x, i_position.y);
}

void Engine::ClampCursorPosition(const Vector2& i_min, const Vector2& i_max)
{
    Vector2 pos = Game::Inputs::GetMousePosition();
    Vector2 newPos = pos;

    newPos.x = Maths::Clamp(newPos.x, i_min.x, i_max.x);
    newPos.y = Maths::Clamp(newPos.y, i_min.y, i_max.y);

    if (newPos != pos)
        SetCursorPosition(newPos);
}

void Engine::EnableInputs()
{
    inputsManager.SetInputsListening(true);
}

void Engine::DisableInputs()
{
    inputsManager.SetInputsListening(false);
}

void Engine::SetUINavigation(bool i_activate) const
{
    ImGuiIO& io = ImGui::GetIO();
    if (i_activate)
    {
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
        if (updateMode & Utils::UpdateFlag_Gaming)
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    }
    else
    {
        io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
    }
}

Game::Camera* Engine::GetEditorGamera() const
{
    return graph->editorCamera;
}

Game::Camera* Engine::GetGameCamera() const
{
    return graph->gameCamera;
}

Game::Transform& Engine::GetEditorCameraTransform() const
{
    return graph->editorCameraman.transform;
}

const UI::Canvas& Engine::GetUICanvas() const
{
    return graph->canvas;
}
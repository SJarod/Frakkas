#pragma once

#include <memory>
#include <list>
#include <functional>
#include <SDL.h>
#include <miniaudio.h>

#include "utils/platform.hpp"
#include "utils/update_flags.hpp"

#include "physic/physic_scene.hpp"

#include "game/entity_manager.hpp"
#include "game/time_manager.hpp"
#include "game/inputs_manager.hpp"
#include "camera.hpp"

namespace Renderer
{
    class Graph;
    namespace LowLevel
    {
        class LowRenderer;
        class Framebuffer;
    }
}
namespace Game
{
    class World;
}

namespace UI
{
    class Canvas;
}

using UpdateEvent = std::function<void()>;

class Engine
{
public:
    Engine();
    ~Engine();

    Game::EntityManager entityManager;
    Physic::PhysicScene physicScene;

    std::unique_ptr<Renderer::LowLevel::LowRenderer> renderer;
    std::unique_ptr<Renderer::LowLevel::Framebuffer> editorFBO;
    std::unique_ptr<Renderer::LowLevel::Framebuffer> gameFBO;
    std::unique_ptr<Game::World> gameWorld;

    std::unique_ptr<Renderer::Graph> graph;
    
    std::list<UpdateEvent> updateEventsHandler;
    InputsEvent editorInputsEvent;

    static ma_engine soundEngine;

    /**
     * Editor main loop. Manage frames and Render entities.
     * Call all UpdateEvent attached to UpdateEventHandler.
     */
    void RunEditor();

    /**
     * Game main loop. Manage frames, Render and Update entities.
     * Call all UpdateEvent attached to UpdateEventHandler.
     */
    void RunGame();

    /**
     * @brief Change engine run mode, and change settings if needed.
     * @param i_flag The new mode to set.
     */
    void SetRunMode(Utils::UpdateFlag i_flag);

    /**
     * @return Current run mode.
     */
    Utils::UpdateFlag GetRunMode() const;

    void SetUINavigation(bool i_activate) const;

    /**
     * Change mouse cursor visibility
     * @param i_visibility The visible state you want
     */
    static void SetCursorVisibility(bool i_visibility = true);

    /**
     * Lock cursor position on the window, useful to move the mouse cursor infinitely. Use deltaMotion infos.
     */
    static void SetCursorGameMode(bool i_gameMode);

    /**
     * Set the mouse cursor to the input position. Relative to window space. Left-Up corner is (0, 0).
     * @param i_position
     */
    static void SetCursorPosition(const Vector2& i_position);

    /**
     * @brief Clamp the mouse cursor position into a rectangle.
     * @param i_min The minimum coordinates to clamp at.
     * @param i_max The maximum coordinates to clamp at.
     */
    static void ClampCursorPosition(const Vector2& i_min, const Vector2& i_max);

    /**
     * @brief Activate keyboard/mouse inputs listening with Inputs class
     */
    void EnableInputs();

    /**
     * @brief Deactivate keyboard/mouse inputs listening with Inputs class
     */
    void DisableInputs();

    /**
     * @return The editor camera from Graph class. never return nullptr.
     */
    Game::Camera* GetEditorGamera() const;

    /**
     * @return The current game camera from Graph class. Can return nullptr.
     */
    Game::Camera* GetGameCamera() const;

    /**
     * @return The transform of the editor camera from Graph class.
     */
    Game::Transform& GetEditorCameraTransform() const;

    /**
     * @return The ui canvas to render ui object on game screen.
     */
    const UI::Canvas& GetUICanvas() const;

private:
    Utils::UpdateFlag updateMode = Utils::UpdateFlag_Editing;

    Game::Time timeManager;
    Game::Inputs inputsManager;

    static SDL_Window* window;
    SDL_GLContext glContext;

    /**
     * Init SDL parameters, link to OpenGL and create a window.
     */
    void InitSDL();

    /**
     * Create ImGui context and setup engine specific parameters
     */
    void InitImGui();

    /**
    * Init SDL audio parameters.
    */
    void InitMiniaudio();

	/**
	* Begin a new frame in engine main loop. Inform other libraries and setup engine parameters.
	*/
    void BeginFrame();

    /**
    * End the current frame in engine main loop. Inform other libraries and swap window buffer.
    */
    bool EndFrame();

    /**
     * @brief Render and Update the imgui context
     */
    void RenderImGui() const;
};
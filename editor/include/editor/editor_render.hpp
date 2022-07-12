#pragma once

#include "editor/menu_bar.hpp"
#include "editor/hierarchy.hpp"
#include "editor/console.hpp"
#include "editor/inspector.hpp"
#include "editor/file_browser.hpp"
#include "editor/scene.hpp"
#include "editor/game_scene.hpp"
#include "editor/debugger.hpp"
#include "editor/resources_viewer.hpp"

#include "engine.hpp"

namespace Renderer::LowLevel
{
    class Framebuffer;
}

namespace Game
{
    class EntityManager;
}

class Engine;

namespace Editor
{
    class EditorRender
    {
        static constexpr float ImGuiWindowPadding = 10.f;
    public:
        EditorRender(Engine& io_engine);

        static bool editingDrag;
        static bool editingText;
        static Vector2 mouseLockPosition;
        static Vector2 gameWindowSize;

        /**
         * @brief Create all the editor interface and allow user to edit its game.
         * @param io_engine The engine to edit.
         */
        void UpdateAndRender(Engine& io_engine);

    private:
        ImGuizmo::OPERATION guizmoOperation = ImGuizmo::OPERATION::BOUNDS;

        MenuBar menuBar;
        Hierarchy hierarchy;
        Console console;
        FileBrowser fileBrowser;
        Scene scene;
        GameScene game;
        Debugger debugger;
        ResourcesViewer resources;
    };
}
#pragma once

class Engine;

namespace Game
{
    class Entity;
    class EntityManager;
}

namespace Renderer
{
    class Graph;
    class Light;
}

class Engine;

namespace Editor
{
    class MenuBar
    {
    public:
        MenuBar() = default;
        ~MenuBar() = default;

        /**
        * @brief Draw the top panel with useful menus.
        * @param io_engine The engine to interact with the entire system.
        * @param o_loadScene An output boolean to notify editor render when the scene is reload.
        * @param io_selectedEntity The entity selected in hierarchy.
        */
        static void OnImGuiRender(Engine& io_engine, bool& o_loadScene, Game::Entity* io_selectedEntity);
        static void FrakkasColors();

    private:
        // Scene management (Save, Load, Reload, New)
        static void FileField(Renderer::Graph& io_graph, bool& o_loadScene);
        static bool CreateScenePopup(Renderer::Graph& io_graph);
        static bool OpenScenePopup(Renderer::Graph& io_graph);

        // Entity editing (copy, past)
        static void EditField(Game::EntityManager& io_entityManager, Game::Entity* io_selectedEntity);

        // Editor style option
        static void OptionsField();

        // Game interaction (Add component, Play, Pause, Stop) 
        static void GameField(Engine& io_engine, bool& o_loadScene);
        static void CreateComponentPopup();

        /**
         * @brief Edit rendering
         * @param io_engine The class that posseses the renderer.
         */
        static void RenderingField(Engine& io_engine);

        /**
         * @brief Edit lighting
         * @param io_engine The class that posseses the graph, the light, and the depth map.
         */
        static void LightingField(Engine& io_engine);

        static void ResourcesField();
    };
}
#pragma once

#include <queue>
#include <filesystem>
#include <list>
#include <functional>

#include "game/entity.hpp"
#include "ui/canvas.hpp"
#include "ui/text.hpp"
#include "ui/panel.hpp"
#include "renderer/light.hpp"

namespace Game
{
    class Component;
    class Camera;
    class Drawable;
    class Collider;
    class Sound;
    class EntityManager;
}

namespace Physic
{
    class PhysicScene;
}

namespace Renderer
{
    using OnSceneLoadEvent = std::function<void()>;

    namespace LowLevel
    {
        class LowRenderer;
    }

    class Graph
    {
    public:
        Graph(Game::EntityManager* io_entityManager, Physic::PhysicScene* i_physicScene, Renderer::LowLevel::LowRenderer* i_renderer);
        ~Graph() { Clear(); };

        static bool playing;

        bool loading = false; // is the graph loading a scene?

        bool lightEnabled = true;
        Renderer::Light light;

        Game::Entity editorCameraman;
        Game::Camera* editorCamera = nullptr;
        Game::Camera* gameCamera = nullptr;

        std::list<OnSceneLoadEvent> sceneLoadEvents;

        static UI::Canvas canvas;
        static UI::Canvas loadingCanvas;

        /**
         * @brief Inform the graph that a new component had been added.
         * If the component is a drawable or a camera, store it into the graph.
         * @param i_newComponent A pointer of the new component to register.
         */
        static void RegisterComponent(Game::Component* i_newComponent);

        /**
         * @brief Inform the graph that a component will be removed.
         * If the component is a drawable or a camera, remove it from the graph.
         * @param i_oldComponent A pointer of the component to unregister.
         */
        static void UnregisterComponent(Game::Component* i_oldComponent);

        /**
         * @brief Clear all data loaded by current scene.
         */
        void Clear();

        /**
         * @brief Renders each entity, using editor camera
         * @param i_renderer the renderer that will draw the entities
         * @param i_aspectRatio the render's aspect ratio
         */
        void RenderEditor(Renderer::LowLevel::LowRenderer& i_renderer, float i_aspectRatio);

        /**
         * @brief Renders each entity, using game camera
         * @param i_renderer the renderer that will draw the entities
         * @param i_aspectRatio the render's aspect ratio
         */
        void RenderGame(Renderer::LowLevel::LowRenderer& i_renderer, float i_aspectRatio);

        /**
         * @brief Create a new scene file with default entities.
         * @param i_scenePath The path and name of the scene.
         * @param io_entityManager The entity manager to create entities.
         * @return true if scene created, false if an error occurred.
         */
        bool CreateScene(std::filesystem::path i_scenePath);

        /**
         * Reload the current scene.
         * @param i_cleaning Should the resources be reloaded?
         */
        void ReloadScene(const bool i_cleaning = true);

        /**
         * Load a scene from a text file.
         * @param i_scenePath Path of the scene.
         * @param i_cleaning Should the resources be reloaded?
         */
        void LoadScene(const std::filesystem::path& i_scenePath, const bool i_cleaning = true);

        /**
         * Set the graph's loading bool to false when the loading is finished.
         */
        void SceneLoadFinished();

        /**
         * @brief Save loading data to load scene at end frame.
         * @param i_cleaning Restore all the resources or not.
         * @param i_loadScenePath The scene to load.
         * @param i_loadingScreenPath The loading screen to be displayed.
         * @param i_minimumLoadTime Delay the loading time to match the specified time.
         */
        void SetLoadingParameters(bool i_cleaning = true,
            const std::filesystem::path& i_loadScenePath = "",
            const std::filesystem::path& i_loadingScreenPath = "",
            const std::string& i_tips = "",
            float i_minimumLoadTime = 0.f);

        /**
         * @brief Load scene from 'loadScenePath' and 'cleaning' internal value.
         * This function is used to avoid reloading scene during game loop.
         */
        void ProcessLoading();

        /**
         * Save a scene into a text file.
         * @param i_sceneName Name of scene, should exists as .kk file.
         */
        void SaveScene() const;

        // Get the scene path from its name (without .kk)
        static std::filesystem::path GetSceneFullPath(const std::string& i_sceneName);

    private:
        Game::EntityManager* entityManager;
        Renderer::LowLevel::LowRenderer* renderer;
        static Physic::PhysicScene* physicScene;

        static bool updateCamera;
        static std::vector<Game::Camera*> gameCameras;
        static std::vector<Game::Drawable*> renderEntities;
        static std::vector<Game::Component*> componentsToStart;
        static std::vector<Game::Sound*> sounds;

        std::filesystem::path currentScenePath;
        std::filesystem::path loadScenePath;

        // loading screen info
        std::filesystem::path loadingScreenPath;
        std::unique_ptr<Game::Panel> panel;
        std::unique_ptr<Game::Text> text;
        std::string tips = "";
        float minimumLoadTime = 0.f;

        bool cleaning = true;

        /**
         * Searches for the first Camera enabled. Set game camera to nullptr if not found.
         * This function is used when the current game camera is disabled.
         */
        void SetGameCameraAuto() noexcept;

        /**
        * @brief Update uniforms as light or camera's matrices before render entities.
        * @param i_renderer The renderer who possesses the shader to set uniform.
        * @param i_aspectRatio The aspect ratio of the screen to get camera projection matrix.
         * @param i_camera The camera to project the scene from.
        */
        void UpdateGlobalUniform(const Renderer::LowLevel::LowRenderer& i_renderer,
                                 float i_aspectRatio,
                                 Game::Camera& i_camera,
                                 const Vector3& i_cameraPos) const noexcept;

        /**
         * @brief Renders each entity for real.
         * In fact, RenderGame and RenderEditor call this function to process the same way.
         * @param i_renderer the renderer that will draw the entities
         */
        void Render(Renderer::LowLevel::LowRenderer& i_renderer, const Game::Camera& i_camera);

        /**
         * @brief Render all enabled collider.
         * @param i_renderer The renderer that will draw the colliders.
         */
        void RenderColliders(Renderer::LowLevel::LowRenderer& i_renderer);
    };
}
#pragma once

#include <ImGuizmo.h>

#include "maths.hpp"

namespace Renderer
{
    class Graph;
    namespace LowLevel
    {
        class Framebuffer;
        class Camera;
    }
}

namespace Game
{
    class Entity;
}
typedef void* ImTextureID;

namespace Editor
{
    class Scene
    {
    public:
        Scene() = default;
        ~Scene() = default;

        /**
         * Tell the editor if user is moving in the scene or not.
         */
        bool isMoving = false;

        /**
         * Position where the cursor is locked when player begin moving.
         */
        Vector2 mouseLockPosition;

        /**
        * @summary Display the ImGui panel
        * @param io_engine the engine to have access to the camera
        * @param i_selectedEntity the selected entity
        * @param i_gizmoOperation variable to edit the gizmo type of a selected entity
        */
        void OnImGuiRender(Engine& io_engine, Game::Entity* i_selectedEntity, ImGuizmo::OPERATION& i_gizmoOperation);

        /**
        * @summary Set resources drag and drop into the window
        * @param io_entityManager the entity manager to create the dropped resource
        * @param io_graph the graph to manage the scene when a .kk file is dropped
        */
        void DragDropResources(Game::EntityManager& io_entityManager, Renderer::Graph& io_graph);

        /**
         * @summary Listen to mouse right click, and enable/disable movement in editor scene.
         */
        void CheckMouseAction();
    };
}
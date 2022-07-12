#pragma once

#include <string>
#include <unordered_map>
#include <ImGuizmo.h>

namespace Game
{
    class Entity;
    class EntityManager;
}

namespace Editor
{
    class Inspector
    {
    public:
        Inspector() = default;
        ~Inspector() = default;

        /**
        * @summary Display the ImGui panel
        */
        static void OnImGuiRender(Game::Entity* io_selectedEntity, ImGuizmo::OPERATION& i_guizmoOperation);

        /**
         * @brief Open a popup window about Add Component
         * @param io_selectedEntity The current entity to add component to.
         */
        static void AddComponentPopup(Game::Entity* io_selectedEntity);

        /**
         * @brief Make an invisible button to handle drop resource path and add component.
         * @param io_selectedEntity The current entity to add component to.
         */
        static void DragDropTargetButton(Game::Entity* io_selectedEntity);
    };
}
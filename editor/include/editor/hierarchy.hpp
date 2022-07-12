#pragma once

#include <string>
#include <unordered_map>
#include <list>

#include "game/entity.hpp"

namespace Game
{
    class EntityManager;
}

namespace Editor
{
    class Hierarchy
    {
    public:
        Hierarchy() = default;
        ~Hierarchy() = default;

        /**
        * @summary Display the ImGui panel
        */
        void OnImGuiRender(Game::EntityManager& io_entityManager);

        Game::Entity* selected = nullptr;

    private:
        Game::Entity* selectedWhenAdd = nullptr;

        std::string selectedLabel;
        bool editSelectedName = false;
        bool openAddEntityPopup = false;
        Game::EntityIdentifier removeID = 0;

        /**
         * @brief Render the entities trees from a map.
         * @param io_entities The map of entities to render in hierarchy
         */
        void RenderEntitiesHierarchy(Game::EntityManager& io_entityManager, const std::unordered_map<Game::EntityIdentifier, Game::Entity*>& io_entities);

        /**
         * @brief Render the entities trees from a list.
         * @param io_entities The list of entities to render in hierarchy
         */
        void RenderEntitiesHierarchy(Game::EntityManager& io_entityManager, const std::list<Game::Entity*>& io_entities);

        /**
         * @brief Render the entity as a tree node in the hierarchy.
         * We setup all the possible interaction with this tree node.
         */
        void RenderEntity(Game::EntityManager& io_entityManager, Game::Entity& io_entity);

        /**
         * @brief Setup all the interactions on the entity tree node.
         * @param io_entity The entity to render.
         */
        void SetupEntityInteraction(Game::EntityManager& io_entityManager, Game::Entity& io_entity, std::string& io_label);

        /**
         * @brief Useful function to set an ImGui Item as a drag drop target to unset entity's parent
         * @param io_entityManager The entity manager which will unlinks the selected entity to its parent.
         */
        void ParentUnsetDragDropTarget(Game::EntityManager& io_entityManager) const;

        /**
         * @brief Open a popup to add various new entity.
         */
        void AddEntityPopup(Game::EntityManager& io_entityManager);

        /**
         * @brief Check if the add-entity-popup can be open, else if unselect inspector entity.
         * @param i_condition The condition to estimate if popup or selection can process.
         */
        void OpenPopupAndUnselect(bool i_condition);
    };
}
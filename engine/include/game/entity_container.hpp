#pragma once

#include "game/entity.hpp"

namespace Game
{
    class Transform;

    /**
     * @brief Class to store entities and give useful utils to Create and Get entity
     */
    class EntityContainer
    {
        static constexpr char CloneSerializationFile[] = "engine/clone.kk";

    public:
        std::vector<std::unique_ptr<Entity>> entities;
        std::unordered_map<EntityIdentifier, Entity*> rootEntities;

        /**
         * @brief Creates an unique-pointer-empty-entity and emplace it in entities array
         * @return the created entity, you can add component to it.
         */
        Entity* CreateEntity(const std::string_view& i_name = "");

        /**
         * @brief Create a copy of an existing entity.
         * The function Serialize the input entity, and read its data to create the copy.
         * @param i_entity The entity to copy. All will be identical except its ID.
         * @return A pointer to the clone.
         */
        Entity* CloneEntity(const Entity& i_entity);

        /**
         * @return The entity with the input ID, thinks about checking if return value is not nullptr.
         */
        Entity* FindEntityWithID(const EntityIdentifier& i_id);

        /**
         * @tparam TComponent Class name of the component
         * @return The first entity with the input component, nullptr if nothing found.
         */
        template<typename TComponent>
        Entity* FindEntityWithComponent();

        /**
         * @brief Find all the entities with the input component and store them in a vector.
         * @tparam TComponent Class name of the component.
         * @return A vector with pointers to all entities who own the component.
         */
        template<typename TComponent>
        std::vector<Entity*> FindEntitiesWithComponent();

        /**
         * @brief Add a parent ot an entity, and update root entities map.
         * @param io_child The child to set the parent
         * @param io_parent The parent, will be informed about its new child
         */
        void SetEntityParent(Entity& io_child, Entity& io_parent);
        /**
         * @brief Set the parent to nullptr for io_child.
         * If parent is not nullptr, we remove io_child from its children.
         * @param io_child The entity to remove parent.
         */
        void UnsetEntityParent(Entity& io_child);

    private:
        static EntityIdentifier maxID;
    };
}

template<typename TComponent>
Game::Entity* Game::EntityContainer::FindEntityWithComponent()
{
    for (const std::unique_ptr<Entity>& entity : entities)
    {
        if (entity->GetComponent<TComponent>())
            return entity.get();
    }

    return nullptr;
}

template<typename TComponent>
std::vector<Game::Entity*> Game::EntityContainer::FindEntitiesWithComponent()
{
    std::vector<Game::Entity*> foundEntities;
    for (const std::unique_ptr<Entity>& entity : entities)
    {
        if (entity->GetComponent<TComponent>())
            foundEntities.emplace_back(entity.get());
    }

    return foundEntities;
}



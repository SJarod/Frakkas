#pragma once

#include <memory>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <string_view>

#include <unordered_map>

#include "entity_container.hpp"

namespace Renderer::LowLevel
{
    class LowRenderer;
}

namespace Resources
{
    class Serializer;
}

namespace Game
{
    class EntityManager
    {
    public:
        EntityManager() = default;
        ~EntityManager() = default;

        EntityContainer entityStore;

        /**
         * OnUpdate every entities
         */
        void Update();

        /**
         * @summary Remove an entity from the entities vector according to its id.
         * @param i_id  The ID of the entity to remove.
         */
        void RemoveEntityAt(const EntityIdentifier& i_id);

        /**
         * @summary Creates an unique-pointer-empty-entity and emplace it in entities array
         * @return the created entity, you can add component to it.
         */
        Entity* CreateEntity(const std::string_view& i_name = "");

        /**
         * @return The entity with the input ID, thinks about checking if return value is not nullptr.
         */
        Entity* FindEntityWithID(const EntityIdentifier& i_id);

        /**
         * @brief read all entities from the file, then create and store them in this manager
         * @param i_file the opened input file
         */
        void LoadEntities(std::ifstream& i_file);

        /**
         * @brief Reset all entities array to 0.
         */
        void ClearEntities();

        const std::vector<std::unique_ptr<Entity>>& GetEntities() const;
        const std::unordered_map<EntityIdentifier, Entity*>& GetRootEntities() const;

    private:
        /**
         * @brief Remove entity from all arrays, and forget about its existence.
         * @param io_entity The entity to forget.
         */
        void ForgetEntity(Entity& io_entity);

        /**
         * @brief Find the entity position in entities array according to its index.
         * @return The index in the entities array. Return -1 if index out of bound.
         */
        int FindEntityIndex(const EntityIdentifier& i_id);
    };
}

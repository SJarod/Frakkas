#include <fstream>

#include "resources/serializer.hpp"
#include "debug/log.hpp"

#include "game/transform.hpp"
#include "game/entity.hpp"
#include "game/entity_container.hpp"

#include "game/entity_manager.hpp"

using namespace Game;

EntityIdentifier EntityContainer::maxID = 1; // Begin at 1, because '0' is index none

Entity* EntityContainer::CreateEntity(const std::string_view& i_name)
{
    entities.emplace_back(std::make_unique<Entity>(maxID++,  i_name));

    Entity* entity = entities.back().get();
    Log::Info("Create new entity : '", entity->name, "'");
    entity->entityStore = this;

    // Set entity as root by default
    rootEntities[entity->GetID()] = entity;

    return entity;
}

Entity* EntityContainer::CloneEntity(const Entity& i_entity)
{
    std::ofstream outFile(CloneSerializationFile);
    Resources::Serializer::Write(outFile, i_entity);
    outFile.close();

    std::ifstream inFile(CloneSerializationFile);
    Entity* cloneEntity = Resources::Serializer::CreateAndReadEntity(inFile, *this);
    inFile.close();

    return cloneEntity;
}

void Game::EntityContainer::SetEntityParent(Entity& io_child, Entity& io_parent)
{
    io_child.transform.parent = &io_parent.transform;

    // Check if parent is accepted
    if (io_child.transform.parent == &io_parent.transform)
    {
        io_child.parent.set(&io_parent);
        io_parent.childs.emplace_back(&io_child);
        rootEntities.erase(io_child.GetID()); // Remove child from root if needed
    }
}

void Game::EntityContainer::UnsetEntityParent(Entity& io_child)
{
    // Check that parent exists
    if (!io_child.parent)
        return;

    Entity& parent = *io_child.parent;
    // Check that parent is indeed a parent
    if (io_child.transform.parent == &parent.transform)
    {
        io_child.transform.parent.get()->RemoveChild(&io_child.transform);
        io_child.transform.parent = nullptr;
        io_child.parent.set(nullptr);
        rootEntities[io_child.GetID()] = &io_child;

        parent.childs.remove_if([&io_child](Entity* parent) { return io_child.GetID() == parent->GetID(); }); // remove if ID is the same
    }
}

Entity* EntityContainer::FindEntityWithID(const EntityIdentifier& i_id)
{
    auto findPredicate = [&i_id](const std::unique_ptr<Entity>& entity)
    {
        return i_id == entity->GetID();
    };
    const auto& it = std::find_if(entities.begin(), entities.end(), findPredicate);

    if (it != entities.end())
        return it->get();

    return nullptr;
}

void EntityManager::Update()
{
    for (int i = entityStore.entities.size()-1; i >= 0; i--)
    {
        Entity* entity = entityStore.entities[i].get();
        for (int j = entity->components.size()-1; j >= 0.f; j--)
        {
            Component* comp = entity->components[j].get();
            if (comp->enabled)
            {
                comp->Start(); // Call OnStart only if needed;
                comp->OnUpdate();
            }
        }
        if (entity->destroy)
            RemoveEntityAt(entity->GetID());
    }
}

void Game::EntityManager::RemoveEntityAt(const EntityIdentifier& i_id)
{
    int index = FindEntityIndex(i_id);

    if (index != -1)
    {
        Log::Info("Remove entity '", entityStore.entities[index]->name, "'");

        ForgetEntity(*entityStore.entities[index]);
    }
}

void Game::EntityManager::ForgetEntity(Entity& io_entity)
{
    // If entity has childs, remove them then recall the remove function with no childs
    if (!io_entity.childs.empty())
    {
        // Remove entity childs
        while (!io_entity.childs.empty())
            RemoveEntityAt(io_entity.childs.front()->GetID());

        io_entity.childs.clear();

        RemoveEntityAt(io_entity.GetID());
        return;
    }

    // If entity has parent, remove child into the parent list
    entityStore.UnsetEntityParent(io_entity);

    // Clear transform childs
    io_entity.transform.ClearChilds();
    if (io_entity.transform.parent)
        io_entity.transform.parent.get()->RemoveChild(&io_entity.transform);

    // Remove entity's components
    for (int i = io_entity.components.size() - 1; i >= 0; i--)
        io_entity.RemoveComponentAt(i);

    // Remove entity from root map
    if (entityStore.rootEntities.find(io_entity.GetID()) != entityStore.rootEntities.end())
        entityStore.rootEntities.erase(io_entity.GetID());

    entityStore.entities.erase(entityStore.entities.begin() + FindEntityIndex(io_entity.GetID()));
}

Entity* EntityManager::CreateEntity(const std::string_view& i_name)
{
    Entity* entity = entityStore.CreateEntity(i_name);
    return entity;
}

Entity* Game::EntityManager::FindEntityWithID(const EntityIdentifier& i_id)
{
    return entityStore.FindEntityWithID(i_id);
}

void EntityManager::LoadEntities(std::ifstream& i_file)
{
    // Reset
    ClearEntities();
    // Load
    while (!i_file.eof())
        Resources::Serializer::CreateAndReadEntity(i_file, entityStore, nullptr);
}

void Game::EntityManager::ClearEntities()
{
    entityStore.entities.clear();
    entityStore.rootEntities.clear();
}

const std::vector<std::unique_ptr<Entity>> & EntityManager::GetEntities() const
{
    return entityStore.entities;
}

const std::unordered_map<EntityIdentifier, Entity*>& Game::EntityManager::GetRootEntities() const
{
    return entityStore.rootEntities;
}

int Game::EntityManager::FindEntityIndex(const EntityIdentifier& i_id)
{
    auto findPredicate = [&i_id](const std::unique_ptr<Entity>& entity)
    {
        return i_id == entity->GetID();
    };

    int index = std::find_if(entityStore.entities.begin(), entityStore.entities.end(), findPredicate) - entityStore.entities.begin();

    if (index >= entityStore.entities.size())
    {
        Log::Warning("Try accessing an entity with index ", index, ". Index out of bound.");
        return -1;
    }

    return index;
}
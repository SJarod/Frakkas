#include <iostream>

#include "renderer/graph.hpp"
#include "physic/physic_scene.hpp"
#include "game/lowcomponent/collider.hpp"

#include "game/entity.hpp"

using namespace Game;

Entity::Entity(const EntityIdentifier& i_id, const std::string_view& i_name)
    :id(i_id)
{
    parent.set(nullptr);
    enabled.set(true);

    parent.setter = [&](Entity* io_value)
    {
        if (io_value == nullptr)
            entityStore->UnsetEntityParent(*this);
        else
            entityStore->SetEntityParent(*this, *io_value);

        parent.set(io_value);
    };

    enabled.setter = [&](bool io_value)
    {
        // Avoid endless call with Component::OnEnabled/OnDisable
        bool prevValue = enabled;
        enabled.set(io_value);

        if (prevValue && !io_value)
        {
            for (int i = components.size()-1; i >= 0; i--)
                components[i]->enabled = false;

            for (Entity* child : childs)
                child->enabled = false;
        }
        else if (!prevValue && io_value)
        {
            for (int i = components.size()-1; i >= 0; i--)
                components[i]->enabled = true;

            for (Entity* child : childs)
                child->enabled = true;
        }
    };

    if (i_name.empty())
        name = "NewEntity_" + std::to_string(id);
    else
        name = i_name;
}

Entity::~Entity()
{
    for(const std::unique_ptr<Component>& comp : components)
        comp->OnDestroy();
}

Component* Entity::AddComponent(std::unique_ptr<Component> comp)
{
    if (!destroy)
    {
        comp->owner = this;
        components.emplace_back(std::move(comp));
        RegisterIntoGraph(components.back().get());
        return components.back().get();
    }
    else
        return nullptr;
}

void Entity::RemoveComponentAt(int index)
{
    // Call OnDestroy event
    components[index]->OnDestroy();
    // Unregister
    UnregisterIntoGraph(components[index].get());
    components.erase(components.begin() + index);
}

void Entity::NotifyCollision(Physic::ECollisionEvent i_collisionType, Collider& i_ownerCollider, Collider& i_otherCollider)
{
    switch (i_collisionType)
    {
    case Physic::ECollisionEvent::COLLISION_ENTER:
        for (int i = components.size()-1; i >= 0; i--)
            if (components[i]->enabled)
                components[i]->OnCollisionEnter(i_ownerCollider, i_otherCollider);
        break;
    case Physic::ECollisionEvent::COLLISION_STAY:
        for (int i = components.size()-1; i >= 0; i--)
            if (components[i]->enabled)
                components[i]->OnCollisionStay(i_ownerCollider, i_otherCollider);
        break;
    case Physic::ECollisionEvent::COLLISION_EXIT:
        for (int i = components.size()-1; i >= 0; i--)
            if (components[i]->enabled)
                components[i]->OnCollisionExit(i_ownerCollider, i_otherCollider);
        break;
    case Physic::ECollisionEvent::TRIGGER_ENTER:
        for (int i = components.size()-1; i >= 0; i--)
            if (components[i]->enabled)
                components[i]->OnTriggerEnter(i_ownerCollider, i_otherCollider);
        break;
    case Physic::ECollisionEvent::TRIGGER_STAY:
        for (int i = components.size()-1; i >= 0; i--)
            if (components[i]->enabled)
                components[i]->OnTriggerStay(i_ownerCollider, i_otherCollider);
        break;
    case Physic::ECollisionEvent::TRIGGER_EXIT:
        for (int i = components.size()-1; i >= 0; i--)
            if (components[i]->enabled)
                components[i]->OnTriggerExit(i_ownerCollider, i_otherCollider);
        break;
    default:
        break;
    }

    for (Entity* child : childs)
        child->NotifyCollision(i_collisionType, i_ownerCollider, i_otherCollider);
}

void Entity::RegisterIntoGraph(Component* i_newComponent) const
{
    if (id != 0)
        Renderer::Graph::RegisterComponent(i_newComponent);
}

void Entity::UnregisterIntoGraph(Component* i_oldComponent) const
{
    if (id != 0)
        Renderer::Graph::UnregisterComponent(i_oldComponent);
}

Entity* Entity::GetRootEntity()
{
    Entity* rootEntity = this;
    while (rootEntity->parent.get())
        rootEntity = rootEntity->parent;

    return rootEntity;
}
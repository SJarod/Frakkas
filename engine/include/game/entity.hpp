#pragma once

#include <vector>
#include <memory>
#include <string>
#include <string_view>

#include "transform.hpp"

#include "utils/properties.hpp"

#include "game/lowcomponent/component.hpp"

namespace Resources
{
    class Serializer;
}

namespace Physic
{
    enum class ECollisionEvent;
}

namespace Game
{
    using EntityIdentifier = uint64_t;

    class Collider;
    class EntityContainer;

    class Entity
    {
    public:
        Entity(const EntityIdentifier& i_id, const std::string_view& i_name = "");
        ~Entity();

        std::string name;
        bool serialize = true; // If false, the entity will not be saved when save the scene.
        Property<bool> enabled;

        bool unsettingParent = false;
        Transform transform;

        std::list<Entity*> childs;
        Property<Entity*> parent;

        EntityContainer* entityStore = nullptr;
        std::vector<std::unique_ptr<Component>> components;

        bool destroy = false;

        /**
         * Push back the input unique component in components array. You must use std::move().
         * Don't add a same component twice, updating process won't be correct.
         * @param comp the component to add. The shared_ptr instance will be copied.
         */
        Component* AddComponent(std::unique_ptr<Component> comp);

        template<typename T>
        T* AddComponent();

        /**
         * @brief Remove a component according to its position in the components array.
         */
        void RemoveComponentAt(int index);

        /**
         * @brief Call the specific collision function in each entity's components.
         * @param i_collisionType The type of collision to call the good function.
         */
        void NotifyCollision(Physic::ECollisionEvent i_collisionType, Collider& i_ownerCollider, Collider& i_otherCollider);

        /**
         * @brief Find a component by its id
         * @tparam T the class type of the component
         * @return the pointer of first T-type component if found, else nullptr
         */
        template<typename T>
        T* GetComponent();

        /**
         * @brief Find all components with the same id
         * @tparam T the class type of the components
         * @return all T-type components in a list, list is empty if not found
         */
        template<typename T>
        std::vector<T*> GetComponents();

        /**
         * @return A reference to the entity property who is a root entity.
         * Return an empty property if this entity does not have parent.
         */
        Entity* GetRootEntity();

        const EntityIdentifier& GetID() const { return id; }
        std::string GetStringID() const { return std::to_string(id); }

    private:

        EntityIdentifier id = 0;

        /**
         * @brief Send the component to the graph register system.
         */
        void RegisterIntoGraph(Component* i_newComponent) const;
        /**
         * @brief Send the component to the graph unregister system.
         */
        void UnregisterIntoGraph(Component* i_oldComponent) const;

    };
}


template<typename T>
T* Game::Entity::AddComponent()
{
    components.emplace_back(std::make_unique<T>());
    components.back()->owner = this;
    RegisterIntoGraph(components.back().get());
    return reinterpret_cast<T*>(components.back().get());
}

template<typename T>
T* Game::Entity::GetComponent() 
{
    std::vector<std::unique_ptr<Component>>::iterator it;
    it = std::find_if(components.begin(), components.end(), [&](const std::unique_ptr<Component>& c){
        return c->GetID() == T::MetaData().className;
    });

    if (it != components.end())
    {
        if (T* comp = reinterpret_cast<T*>(it->get()))
            return comp;
    }

    return nullptr;
}

template<typename T>
std::vector<T*> Game::Entity::GetComponents()
{
    std::vector<T*> comps;
    for(std::vector<std::unique_ptr<Component>>::iterator it = components.begin(); it != components.end(); it++)
    {
        if (it->get()->GetID() == T::MetaData().className)
        {
            if (T* comp = reinterpret_cast<T*>(it->get()))
                comps.emplace_back(comp);
        }
    }

    return comps;
}

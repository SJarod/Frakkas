#pragma once

#include <string>
#include <utility>
#include <vector>

#include "utils/properties.hpp"

#include "game/reflection/data_descriptor.hpp"

namespace Resources
{
    class Serializer;
}

namespace Game
{
    class Entity;
    class EntityContainer;
    class Transform;
    class Collider;
    class Component
    {
    public:
        Component();
        virtual ~Component() = default;

        Property<bool> enabled;
        Property<Entity*> owner;

        /**
         * @return A registry array which store all Component metadata.
         */
        static std::vector<ClassMetaData*>& GetRegistry();

        /**
         * @param i_compName The name of the component class
         * @return The class meta data that has the same className that the input component name.
         */
        static ClassMetaData* FindMetaData(const std::string& i_compName);

        /**
         * @return an ID that identify the component. Useful to find a specific component in a bunch of another components.
         */
        virtual const std::string& GetID() const = 0;
        
        /**
         * @summary Called when the owner is included into the engine's EntityManager,
         * so it is called once even if component should be disabled,
         * and beware that other entities may not be set at this moment.
         */
        virtual void OnStart() {};

        /**
         * @brief Check if component is started, if not make it started !
         */
        void Start();

        /**
         * @return true if the component is started. Which means that the component has been updated at least once.
         */
        bool IsStarted() const;

        /**
         * @Summary Called every frame in the game loop if component enabled.
         * The common function to write game script.
         */
        virtual void OnUpdate() {};

        /**
         * Called when component is removed, or when owner is deleted.
         * Called even if component is disabled.
         */
        virtual void OnDestroy(){};

        /**
         * @brief Called when change enabled state. Could be consider as a OnStart() but can be call many times.
         * Not called when entity started
         */
        virtual void OnEnable() {};
        /**
         * @brief Call when change enabled state. Could be consider as a OnDestroy() but can be call many times.
         * Not called when entity destroyed
         */
        virtual void OnDisable() {};

        /**
         * @brief Called when two colliders collide. You should have a non static collider.
         * @param i_ownerCollider The collider of the entity which owns this component as well.
         * @param i_otherCollider The collider of the colliding entity.
         */
        virtual void OnCollisionEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider){};

        /**
         * @brief Called when two colliders are overlapping. You should have a non static collider.
         * @param i_ownerCollider The collider of the entity which owns this component as well.
         * @param i_otherCollider The collider of the colliding entity.
         */
        virtual void OnCollisionStay(const Collider& i_ownerCollider, const Collider& i_otherCollider){};

        /**
         * @brief Called when two colliders are no colliding anymore. You should have a non static collider.
         * @param i_ownerCollider The collider of the entity which owns this component as well.
         * @param i_otherCollider The collider of the colliding entity.
         */
        virtual void OnCollisionExit(const Collider& i_ownerCollider, const Collider& i_otherCollider){};

        /**
         * @brief Called when two colliders collide, and there is at least one trigger collider.
         * @param i_ownerCollider The collider of the entity which owns this component as well.
         * @param i_otherCollider The collider of the colliding entity.
         */
        virtual void OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider) {};

        /**
         * @brief Called when two colliders are overlapping, and there is at least one trigger collider.
         * @param i_ownerCollider The collider of the entity which owns this component as well.
         * @param i_otherCollider The collider of the colliding entity.
         */
        virtual void OnTriggerStay(const Collider& i_ownerCollider, const Collider& i_otherCollider) {};

        /**
         * @brief Called when two colliders are no colliding anymore, and there is at least one trigger collider.
         * @param i_ownerCollider The collider of the entity which owns this component as well.
         * @param i_otherCollider The collider of the colliding entity.
         */
        virtual void OnTriggerExit(const Collider& i_ownerCollider, const Collider& i_otherCollider) {};

        /**
         * @return A reference to owner's transform position.
         */
        Property<Vector3>& Position() const;
        /**
         * @return A reference to owner's transform scale.
         */
        Property<Vector3>& Scale() const;
        /**
         * @return A reference to owner's transform rotation.
         */
        Property<Vector3>& Rotation() const;

        /**
         * @return A reference to owner's transform.
         */
        Transform& GetTransform() const;

        /**
         * @return A reference to the transform of the parent entity which is a root entity.
         * Return GetTransform() if the owner does not have parent.
         */
        Transform& GetRootTransform() const;

        /**
         * @brief an abstract function, every inheretid components should define this function with KK_GENERATE_COMPONENT
         * @return the meta data of the class, in other words the list of member variable readable for the engine.
         */
        virtual const ClassMetaData& GetMetaData() const = 0;

    protected:

        /**
         * @Summary Set or replace the current entity which hold the component. You can unset the entity by sending nullptr.
         * @param entity an entity floating pointer, should be store somewhere else as smart pointer.
         */
        virtual void SetOwner(Entity* entity);

        /**
         * @return The owner's entity container that gives access to all entities and useful find function.
         */
        EntityContainer& GetEntityContainer() const;

    private:
        bool isStarted = false;
    };
}

#pragma once

#include "collider/sphere_collider.hpp"

#include "component_generator.hpp"

namespace Game
{
    KK_COMPONENT(PhysicPlayer);

        float speed = 800.f;
        float jumpForce = 900.f;
        float gravityFactor = 3.f;

        void OnStart() override;

        void OnUpdate() override;

        void OnCollisionEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider)override {};

        void OnCollisionStay(const Collider& i_ownerCollider, const Collider& i_otherCollider)override {};

        void OnCollisionExit(const Collider& i_ownerCollider, const Collider& i_otherCollider)override {};

        void OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider) override {};

        void OnTriggerStay(const Collider& i_ownerCollider, const Collider& i_otherCollider) override {};

        void OnTriggerExit(const Collider& i_ownerCollider, const Collider& i_otherCollider) override {};

        SphereCollider* rigidBody = nullptr;

    KK_COMPONENT_END
}

void OnGravityChanged(unsigned char* io_component);

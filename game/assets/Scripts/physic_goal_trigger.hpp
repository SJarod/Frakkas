#pragma once

#include "component_generator.hpp"

namespace Game
{
    KK_COMPONENT(PhysicGoalTrigger);

        void OnStart() override;

        void OnUpdate() override;

        void OnDestroy()override {};

        void OnEnable() override {};

        void OnDisable() override {};

        void OnCollisionEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider)override {};

        void OnCollisionStay(const Collider& i_ownerCollider, const Collider& i_otherCollider)override {};

        void OnCollisionExit(const Collider& i_ownerCollider, const Collider& i_otherCollider)override {};

        void OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider) override;

        void OnTriggerStay(const Collider& i_ownerCollider, const Collider& i_otherCollider) override {};

        void OnTriggerExit(const Collider& i_ownerCollider, const Collider& i_otherCollider) override {};

    KK_COMPONENT_END
}

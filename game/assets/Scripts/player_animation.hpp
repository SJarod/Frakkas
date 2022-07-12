#pragma once

#include "drawable/animated_draw.hpp"
#include "component_generator.hpp"

enum class EAnimState
{
    NONE,
    IDLE,
    WALK,
    RUN
};

namespace Game
{
    KK_COMPONENT(PlayerAnimation);

        AnimatedDraw* ad = nullptr;
        EAnimState state = EAnimState::NONE;
        int attack = 0;

        void SetState(const EAnimState i_state);

        /// You can remove the functions you don't need.
        void OnStart() override;

        void OnUpdate() override;

        void OnDestroy()override {};

        void OnEnable() override {};

        void OnDisable() override {};

        void OnCollisionEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider)override {};

        void OnCollisionStay(const Collider& i_ownerCollider, const Collider& i_otherCollider)override {};

        void OnCollisionExit(const Collider& i_ownerCollider, const Collider& i_otherCollider)override {};

        void OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider) override {};

        void OnTriggerStay(const Collider& i_ownerCollider, const Collider& i_otherCollider) override {};

        void OnTriggerExit(const Collider& i_ownerCollider, const Collider& i_otherCollider) override {};

    KK_COMPONENT_END
}

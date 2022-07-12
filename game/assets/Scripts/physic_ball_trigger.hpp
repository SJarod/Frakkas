#pragma once

#include "physic_ball_object.hpp"

#include "component_generator.hpp"

namespace Game
{
    KK_COMPONENT(PhysicBallTrigger);

        Vector3 ballPosition = {-25.f, 5.f, 0.f};
        Entity* ball = nullptr;

        PhysicBallObject* currentBall = nullptr;
        float maxScale = 3.f;

        bool createBall = false;
        float createTimer = 5.f;

        void OnStart() override;
        void OnUpdate() override;

        void OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider) override;

        void OnTriggerStay(const Collider& i_ownerCollider, const Collider& i_otherCollider) override;

        void OnTriggerExit(const Collider& i_ownerCollider, const Collider& i_otherCollider) override;

    KK_COMPONENT_END
}

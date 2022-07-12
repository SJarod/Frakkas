#pragma once

#include "maths.hpp"
#include "game/transform.hpp"

#include "component_generator.hpp"

namespace Game
{
    KK_COMPONENT(FollowCamera)

        Transform* playerTransform = nullptr;
        Vector3 offset = Vector3::zero;
        float lerpFactor = 0.08f;

        void OnStart() override;

        void OnUpdate() override;
    KK_COMPONENT_END
}

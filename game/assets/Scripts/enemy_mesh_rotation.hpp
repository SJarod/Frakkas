#pragma once

#include "component_generator.hpp"

namespace Game
{
    KK_COMPONENT(EnemyMeshRotation);

        Vector3 fromTo;

        /// You can remove the functions you don't need.
        void OnStart() override;

        void OnUpdate() override;

    KK_COMPONENT_END
}

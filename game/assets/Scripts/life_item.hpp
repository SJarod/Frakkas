#pragma once

#include "component_generator.hpp"

namespace Game
{
    KK_COMPONENT(LifeItem);

        int life = 5;
        Vector3 startPosition;

        void OnStart() override;
        void OnUpdate() override;

    KK_COMPONENT_END
}

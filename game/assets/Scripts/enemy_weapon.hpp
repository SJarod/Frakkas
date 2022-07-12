#pragma once

#include "component_generator.hpp"

namespace Game
{
    KK_COMPONENT(EnemyWeapon);

        int damage = 0;

        void OnStart() override;

    KK_COMPONENT_END
}

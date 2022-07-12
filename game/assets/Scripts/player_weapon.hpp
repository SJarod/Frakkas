#pragma once

#include "component_generator.hpp"

namespace Game
{
    class Player;

    KK_COMPONENT(PlayerWeapon);

        int damage = 0;

        void OnStart() override;

        void OnUpdate() override;

    private:
        Player* player;

    KK_COMPONENT_END

}

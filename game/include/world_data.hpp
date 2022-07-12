#pragma once

#include "game/world.hpp"

namespace Game
{
    struct FrakkarenaWorldData : public WorldData
    {
        int playerLife = 20;

        int damageLevel = 0;
        int lifeLevel = 0;
        int speedLevel = 0;
        int damageSpeedLevel = 0;

        int skeletonCount = 0;
        int golemCount = 0;
        int comboScore = 0;
        int levelComboScore = 0;
        int level = 0;

        int score = 0;
    };
}

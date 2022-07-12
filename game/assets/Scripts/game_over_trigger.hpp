#pragma once

#include <functional>

#include "component_generator.hpp"

namespace Game
{
    using GameOverEvent = std::function<void()>;

    KK_COMPONENT(GameOverTrigger);

        void OnStart() override;

        void OnEnable() override;

    private:
        int sScore = 0;
        int gScore = 0;
        int levelScore = 0;

        void ComputeScore();

    KK_COMPONENT_END
}

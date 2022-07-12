#pragma once

#include "sound.hpp"

#include "component_generator.hpp"

namespace Game
{
    class AnimatedDraw;

    KK_COMPONENT(MenuScript);

        AnimatedDraw* playerAnimation = nullptr;

        Sound* BGM = nullptr;
        Sound* BGM1 = nullptr;
        Sound* BGM2 = nullptr;
        Sound* BGM3 = nullptr;
        Sound* buttonSound = nullptr;

        void OnStart() override;

        void OnUpdate() override;

        static void ResetWorldData();

    KK_COMPONENT_END
}

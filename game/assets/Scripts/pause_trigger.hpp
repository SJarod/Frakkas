#pragma once

#include <functional>

#include "component_generator.hpp"

namespace Game
{
    using PauseEvent = std::function<void()>;

    KK_COMPONENT(PauseTrigger);


        void OnStart() override;

        void OnEnable() override;

        void OnDisable() override;

    private:

    KK_COMPONENT_END
}

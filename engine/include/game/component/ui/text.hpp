#pragma once

#include "component_generator.hpp"
#include "game/lowcomponent/ui_object.hpp"

namespace Game
{
    KK_COMPONENT_FROM(Text, UIObject)

        std::string text = "hello world!";

        void Render(ImDrawList& io_dl, const Vector2& i_origin, const Vector2& i_windowSize) const override;

    KK_COMPONENT_END
}
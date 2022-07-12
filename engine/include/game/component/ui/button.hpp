#pragma once

#include <list>
#include <functional>

#include "component_generator.hpp"
#include "game/lowcomponent/ui_object.hpp"

namespace Game
{
    using ButtonClickEvent = std::function<void()>;

    KK_COMPONENT_FROM(Button, UIObject)

        float textScale = 1.f;
        std::string text = "hello world!";
        Vector4 idleColor = { 0.8f, 0.f, 0.f, 1.f};
        Vector4 hoveredColor = { 0.8f, 0.4f, 0.4f, 1.f};
        Vector4 activatedColor = { 1.0f, 0.f, 0.f, 1.f};

        void AddClickEvent(const ButtonClickEvent& i_event);

        void Render(ImDrawList& io_dl, const Vector2& i_origin, const Vector2& i_windowSize) const override;

    private:
        std::list<ButtonClickEvent> clickEvent;

    KK_COMPONENT_END
}
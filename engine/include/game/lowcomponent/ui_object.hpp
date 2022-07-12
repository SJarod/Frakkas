#pragma once

#include "imgui.h"

#include "component_generator.hpp"

struct ImDrawList;

namespace Game
{
    using UIObjectIdentifier = uint64_t;

    KK_PRIVATE_COMPONENT(UIObject)

        UIObjectIdentifier id;

        Vector2 position;
        Vector2 scale = { 1.f, 1.f };

        Vector4 tint = { 0.f, 0.f, 0.f, 1.f };

        /**
         * @brief Update screen position and scale, then Render UIObject.
         * @param io_dl The ImGui drawlist to draw shapes or texture.
         * @param i_origin The top-left corner position of the game window.
         * @param i_windowSize The size of the game window.
         */
        void UpdateAndRender(ImDrawList& io_dl, const Vector2& i_origin, const Vector2& i_windowSize);

    protected:
        Vector2 screenPosition;
        Vector2 screenScale;
        float averageScale = 1.f;

        /**
         * @brief Use ImGui utils to render a simple UI Object, Should be override in all UIObject child.
         * @param io_dl The ImGui drawlist to draw shapes or texture.
         * @param i_origin The top-left corner position of the game window.
         * @param i_windowSize The size of the game window.
         */
        virtual void Render(ImDrawList& io_dl, const Vector2& i_origin, const Vector2& i_windowSize) const {};

        // An average value to scale UIObject according to window size.
        static constexpr float baseWindowScale = 600.f;

    KK_COMPONENT_END
}

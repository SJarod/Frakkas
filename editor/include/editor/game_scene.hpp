#pragma once

#include "renderer/lowlevel/lowrenderer.hpp"

namespace Editor
{
    class GameScene
    {
    public:
        GameScene() = default;
        ~GameScene() = default;

        /**
         * Position where the cursor is locked when user begin playing.
         */
        Vector2 mouseLockPosition;

        /**
        * @brief Draw the game framebuffer texture, and allow interactions with this window.
        */
        void OnImGuiRender(Engine& io_engine);
    };
}
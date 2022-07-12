#pragma once

#include <imgui.h>
#include <memory>
#include <vector>
#include <filesystem>

#include "resources/texture.hpp"
#include "maths.hpp"
#include "game/lowcomponent/ui_object.hpp"
#include "ui/image_panel.hpp"

namespace UI
{
    class Canvas
    {
    public:
        static constexpr char defaultLoadingScreen[] = "editor/assets/load_screen_1.png";

        Canvas();
        ~Canvas() = default;

        ImGuiWindowFlags windowSettings;

        /**
         * @brief Set UIObject ID and add it to rendering loop
         * @param io_uiObject The new built-UIObject to add.
         */
        void AddUIObject(Game::UIObject* io_uiObject);

        /**
         * @brief Check if UIObject is in objects list and remove it.
         * @param i_uiObject The UIObject to remove from rendering loop.
         */
        void RemoveUIObject(const Game::UIObject* i_uiObject);

        /**
         * @brief Remove all the UIObjects from rendering loop.
         */
        void Clear();

        /**
         * @brief Begin an ImGui window and render ui objects
         */
        void BeginAndRender() const;

        /**
         * @brief Render ui objects, an imgui window has to be opened !
         */
        void Render(const Vector2& i_offset = Vector2()) const;

        /**
         * @brief Initialize the first loading screen
         */
        void StartLoadingScreen(const std::filesystem::path& i_loadingScreenPath);

        /**
         * @brief Apply a fade effect on the loading screen texture
         * 
         * @return fading finished
         */
        bool FadeAway();

    private:
        std::vector<Game::UIObject*> objects;

        Game::UIObjectIdentifier maxID = 1;

        mutable Game::ImagePanel loadingScreen;
    };
}
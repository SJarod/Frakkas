#pragma once

#include <memory>
#include <functional>

#include "game/lowcomponent/ui_object.hpp"
#include "component_generator.hpp"

namespace Resources
{
    class Texture;
}

namespace Game
{
    using ImageButtonClickEvent = std::function<void()>;

    KK_COMPONENT_FROM(ImageButton, UIObject)

        std::string texturePath;
        bool imageFlip = false;
        Vector4 idleColor = { 0.8f, 0.8f, 0.8f, 1.f};
        Vector4 hoveredColor = {1.f, 1.f, 1.f, 1.f};
        Vector4 activatedColor = {1.f, 1.f, 0.2f, 1.f};

        void AddClickEvent(const ImageButtonClickEvent& i_event);

        void SetTexture(const std::string& i_texturePath);

        void Render(ImDrawList& io_dl, const Vector2& i_origin, const Vector2& i_windowSize) const override;

    private:
        std::list<ImageButtonClickEvent> clickEvent;
        std::shared_ptr<Resources::Texture> texture;

    KK_COMPONENT_END
}

void OnImageButtonTextureSet(unsigned char* io_component);
void OnImageButtonTextureDrop(unsigned char* io_component, void* io_data);
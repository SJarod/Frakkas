#pragma once

#include "renderer/model.hpp"
#include "game/lowcomponent/drawable.hpp"

#include "component_generator.hpp"

namespace Game
{
    KK_COMPONENT_FROM(StaticDraw, Drawable)

        Renderer::Model model;

        std::string meshPath = "none";
        std::string texturePath = "none";

        void SetMesh(const std::string& i_path) override;
        void SetTexture(const std::string& i_path, bool i_flip) override;

    protected:
        void Draw(Renderer::LowLevel::LowRenderer& i_renderer, const Renderer::Light& i_light, const Game::Transform& i_entityTransform = {}) override;

        void DrawDepthMap(Renderer::LowLevel::LowRenderer& i_renderer, const Game::Transform& i_entityTransform) override;

        bool IsOpaque() const override;

    KK_COMPONENT_END
}

void DropOnStaticDrawComponent(unsigned char* io_component, void* io_dropData);
void OnMeshPathUpdate(unsigned char* io_component);
void OnTexturePathUpdate(unsigned char* io_component);

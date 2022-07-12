#pragma once

#include "renderer/skeletal_model.hpp"
#include "animation/animation_graph.hpp"
#include "game/lowcomponent/drawable.hpp"

#include "component_generator.hpp"

namespace Game
{
    KK_COMPONENT_FROM(AnimatedDraw, Drawable)

        Renderer::SkeletalModel skmodel;
        Animation::AnimationGraph animGraph;

        std::string meshPath = "none";
        std::string texturePath = "none";

        void SetMesh(const std::string& i_path) override;
        void SetTexture(const std::string& i_path, bool i_flip) override;

        /**
         * @brief Set a new animation resource to this drawable object.
         */
        void SetAnimation(const std::string& i_path);

protected:
    void OnStart() override;

    void OnUpdate() override;

    void Draw(Renderer::LowLevel::LowRenderer& i_renderer, const Renderer::Light& i_light, const Game::Transform& i_entityTransform = {}) override;

    void DrawDepthMap(Renderer::LowLevel::LowRenderer& i_renderer, const Game::Transform& i_entityTransform) override;

    bool IsOpaque() const override;

    KK_COMPONENT_END
}

void DropOnAnimatedDrawComponentMesh(unsigned char* io_component, void* io_dropData);
void DropOnAnimatedDrawComponentTexture(unsigned char* io_component, void* io_dropData);
void OnAnimationMeshPathUpdate(unsigned char* io_component);
void OnAnimationTexturePathUpdate(unsigned char* io_component);
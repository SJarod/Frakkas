#pragma once

#include "component_generator.hpp"
#include "game/lowcomponent/collider.hpp"
#include "renderer/lowlevel/lowrenderer.hpp"

namespace Game
{
    KK_COMPONENT_FROM(SphereCollider, Collider);

        void ApplyEntityUpdate() override;

        void DebugDraw(Renderer::LowLevel::LowRenderer& i_renderer, const Game::Transform& i_entityTransform) const override;

    private:
        float prevRadius = 1.f;

        /**
         * Check collider scale, and create new scaled sphere if needed.
         */
        void UpdateSphereShape();

    KK_COMPONENT_END
}
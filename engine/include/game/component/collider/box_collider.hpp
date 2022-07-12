#pragma once

#include "component_generator.hpp"
#include "game/lowcomponent/collider.hpp"

namespace Game
{
    KK_COMPONENT_FROM(BoxCollider, Collider);

        void ApplyEntityUpdate() override;

        void DebugDraw(Renderer::LowLevel::LowRenderer& i_renderer, const Game::Transform& i_entityTransform) const override;

    private:
        Vector3 prevExtension;

        /**
         * Check collider scale and create new scaled shape if needed.
         */
        void UpdateBoxScale();

    KK_COMPONENT_END
}
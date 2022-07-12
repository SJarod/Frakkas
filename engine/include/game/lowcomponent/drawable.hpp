#pragma once

#include "component_generator.hpp"

namespace Renderer
{
	class Light;

	namespace LowLevel
	{
		class LowRenderer;
	}
}

namespace Game
{
	KK_PRIVATE_COMPONENT(Drawable)
		/**
		 * @brief Draw this drawable.
		 */
		virtual void Draw(Renderer::LowLevel::LowRenderer& i_renderer, const Renderer::Light& i_light, const Game::Transform& i_entityTransform) {}
		/**
		 * @brief Draw this drawable to the depth map for shadow mapping.
		 */
		virtual void DrawDepthMap(Renderer::LowLevel::LowRenderer& i_renderer, const Game::Transform& i_entityTransform) {}

        /**
         * @brief Set a new mesh resource to this drawable object.
         */
        virtual void SetMesh(const std::string& i_path) {};
        /**
         * @brief Set a new texture resource to this drawable object.
         */
        virtual void SetTexture(const std::string& i_path, bool i_flip) {};

		/**
		 * @brief Is this drawable's texture opaque or transparent?
		 */
		virtual bool IsOpaque() const { return true; };

	KK_COMPONENT_END
}
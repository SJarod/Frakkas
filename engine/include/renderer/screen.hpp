#pragma once

#include "renderer/render_object.hpp"

namespace Renderer
{
	class ScreenQuad : public Renderer::RenderObject
	{
	public:
		GLuint VBO = 0;
		GLuint VAO = 0;

		/**
		 * Create a point and store it into a VAO.
		 * @param i_postprocess Is this quad used for post processing?
		 */
		ScreenQuad(const bool i_postprocess = false);
		~ScreenQuad();
	};
}
#pragma once

#include "renderer/render_object.hpp"

namespace Renderer
{
	class Point : public Renderer::RenderObject
	{
	public:
		GLuint VAO = 0;

		/**
		 * Create a point and store it into a VAO.
		 */
		Point();
		~Point();
	};
}
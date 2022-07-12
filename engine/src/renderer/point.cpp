#include "renderer/point.hpp"

Renderer::Point::Point()
	: RenderObject("pointShader", "engine/shaders/point")
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttrib3f(0, 0.f, 0.f, 0.f);
	glBindVertexArray(0);
}

Renderer::Point::~Point()
{
	glDeleteVertexArrays(1, &VAO);
}
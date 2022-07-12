#include "renderer/screen.hpp"

Renderer::ScreenQuad::ScreenQuad(const bool i_postprocess)
	: RenderObject(i_postprocess ?
		RenderObject{ "postprocessShader", "engine/shaders/postprocess" } :
		RenderObject{ "screenShader", "engine/shaders/screen" })
{
	float vertices[] = {
		-1.f, -1.f,
		 1.f, -1.f,
		-1.f,  1.f,

		-1.f,  1.f,
		 1.f, -1.f,
		 1.f,  1.f
	};

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
	glBindVertexArray(0);
}

Renderer::ScreenQuad::~ScreenQuad()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}
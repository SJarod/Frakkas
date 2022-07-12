#pragma once

#include <memory>

#include "resources/program_shader.hpp"

namespace Renderer
{
	class Material
	{
	public:
		Vector3 ambient = { 0.8f, 0.8f, 0.8f };
		Vector3 diffuse = { 1.f, 1.f, 1.f };
		Vector3 specular = { 1.f, 1.f, 1.f };
		Vector3 emissive = { 0.f, 0.f, 0.f };
        Vector4 tint = { 0.f, 0.f, 0.f, 1.f };
		float shininess = 32.f;
	};

	/**
	 * RenderObjects are renderable.
	 */
	class RenderObject
	{
	public:
		std::shared_ptr<Resources::Shader> shader;

		/**
		 * Create a RenderObject using a set of shaders.
		 */
		RenderObject(const std::string& i_name,
			const std::string& i_shaderFilePath,
			const std::initializer_list<const std::string>& i_defines = {});

		/**
		 * Create a RenderObject using seperate shaders.
		 */
		RenderObject(const std::string& i_name,
			const std::string& i_vertexShaderFilePath,
			const std::string& i_fragmentShaderFilePath,
			const std::initializer_list<const std::string>& i_defines = {});
	};

	class SceneObject
	{
	public:
		std::shared_ptr<Resources::Shader> lightDepthShader;

		Material material;

		SceneObject(const std::string& i_name, const std::initializer_list<const std::string>& i_defines = {});
	};
}
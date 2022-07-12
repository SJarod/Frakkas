#include "resources/resources_manager.hpp"

#include "renderer/render_object.hpp"

Renderer::RenderObject::RenderObject(const std::string& i_name,
	const std::string & i_shaderFilePath,
	const std::initializer_list<const std::string>& i_defines)
{
	shader = ResourcesManager::LoadResource<Resources::Shader>(i_name, i_shaderFilePath, i_defines);
}

Renderer::RenderObject::RenderObject(const std::string& i_name,
	const std::string& i_vertexShaderFilePath,
	const std::string& i_fragmentShaderFilePath,
	const std::initializer_list<const std::string>& i_defines)
{
	shader = ResourcesManager::LoadResource<Resources::Shader>(i_name,
		i_vertexShaderFilePath,
		i_fragmentShaderFilePath,
		i_defines);
}

Renderer::SceneObject::SceneObject(const std::string& i_name, const std::initializer_list<const std::string>& i_defines)
{
	lightDepthShader = ResourcesManager::LoadResource<Resources::Shader>(i_name,
		"engine/shaders/light_depth", i_defines);
}
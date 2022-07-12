#include "resources/mesh.hpp"
#include "resources/resources_manager.hpp"

#include "renderer/model.hpp"

Renderer::Model::Model()
	: RenderObject("basicShader", "engine/shaders/basic"), SceneObject("depthmapShader")
{}

void Renderer::Model::SetMeshFromFile(const std::string& i_meshFilename)
{
	mesh = ResourcesManager::LoadResource<Mesh>(i_meshFilename);
}

void Renderer::Model::SetMeshFromFile(const std::string& i_meshFilename, const std::string& i_textureFilename, const bool i_flipTexture)
{
	mesh = ResourcesManager::LoadResource<Mesh>(i_meshFilename);
	SetTexture(i_textureFilename, i_flipTexture);
}

void Renderer::Model::SetTexture(const std::string& i_textureFilename, const bool i_flipTexture)
{
	if (!mesh.lock())
		return;

	flipTexture = i_flipTexture;
	textureName = i_textureFilename;

	diffuseTex = ResourcesManager::LoadResource<Texture>(i_textureFilename, i_flipTexture);
}
#include "resources/resources_manager.hpp"

#include "renderer/skeletal_model.hpp"

Renderer::SkeletalModel::SkeletalModel()
	: RenderObject("basicShader_skinning", "engine/shaders/basic", { "#define SKINNING\n" }),
	SceneObject("depthmapShader_skinning", { "#define SKINNING\n" })
{}

void Renderer::SkeletalModel::SetSkeletalMeshFromFile(const std::string& i_skmeshFilename)
{
	skmesh = ResourcesManager::LoadResource<Resources::SkeletalMesh>(i_skmeshFilename);
}

void Renderer::SkeletalModel::SetSkeletalMeshFromFile(const std::string& i_skmeshFilename, const std::string& i_textureFilename, const bool i_flipTexture)
{
	skmesh = ResourcesManager::LoadResource<Resources::SkeletalMesh>(i_skmeshFilename);
	SetTexture(i_textureFilename, i_flipTexture);
}

void Renderer::SkeletalModel::SetTexture(const std::string& i_textureFilename, const bool i_flipTexture)
{
	if (!skmesh.lock())
		return;

	flipTexture = i_flipTexture;
	textureName = i_textureFilename;

	diffuseTex = ResourcesManager::LoadResource<Texture>(i_textureFilename, i_flipTexture);
}

void Renderer::SkeletalModel::AddSocket(const std::string& i_meshFilename, const std::string& i_textureFilename, const bool i_flipTexture, int i_boneID, const Game::Transform& i_transform)
{
	if (skmesh.expired())
		return;

	if (skmesh.lock()->loaded.test())
	{
		Socket s;
		s.boneID = i_boneID;
		s.model.SetMeshFromFile(i_meshFilename);
		s.model.SetTexture(i_textureFilename, i_flipTexture);
		s.transform = i_transform;
		if (s.model.mesh.expired())
			return;
		sockets.push_back(s);
	}
	else
	{
		ThreadPool::AddTask([this, i_meshFilename, i_textureFilename, i_flipTexture, i_boneID, i_transform]() {
			AddSocket(i_meshFilename, i_textureFilename, i_flipTexture, i_boneID, i_transform);
			});
	}
}

void Renderer::SkeletalModel::RemoveSocket(int i_boneID)
{
	for (std::list<Socket>::iterator it = sockets.begin(); it != sockets.end(); ++it)
	{
		if (it->boneID == i_boneID)
		{
			sockets.erase(it);
			break;
		}
	}
}
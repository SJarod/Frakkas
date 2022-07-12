#include <cassert>
#include <filesystem>

#include "game/entity.hpp"
#include "game/time_manager.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/light.hpp"
#include "resources/resources_manager.hpp"
#include "resources/skeletal_mesh.hpp"

#include "utils/dragdrop_constants.hpp"
#include "renderer/model.hpp"
#include "drawable/animated_draw.hpp"

KK_COMPONENT_IMPL_BEGIN(AnimatedDraw)
KK_FIELD_PUSH(AnimatedDraw, meshPath, EDataType::STRING)
KK_FIELD_CHANGECALLBACK(OnAnimationMeshPathUpdate)
KK_FIELD_DROPTARGET(Utils::ResourcePathDragDropID, DropOnAnimatedDrawComponentMesh)

KK_FIELD_PUSH(AnimatedDraw, texturePath, EDataType::STRING)
KK_FIELD_CHANGECALLBACK(OnAnimationTexturePathUpdate)
KK_FIELD_DROPTARGET(Utils::ResourcePathDragDropID, DropOnAnimatedDrawComponentTexture)

KK_FIELD_PUSH(AnimatedDraw, skmodel.material.ambient, EDataType::FLOAT)
KK_FIELD_COUNT(3)
KK_FIELD_RANGE(0.f, 1.f)
KK_FIELD_PUSH(AnimatedDraw, skmodel.material.diffuse, EDataType::FLOAT)
KK_FIELD_COUNT(3)
KK_FIELD_RANGE(0.f, 1.f)
KK_FIELD_PUSH(AnimatedDraw, skmodel.material.specular, EDataType::FLOAT)
KK_FIELD_COUNT(3)
KK_FIELD_RANGE(0.f, 1.f)
KK_FIELD_PUSH(AnimatedDraw, skmodel.material.emissive, EDataType::FLOAT)
KK_FIELD_COUNT(3)
KK_FIELD_RANGE(0.f, 1.f)
KK_FIELD_PUSH(AnimatedDraw, skmodel.material.tint, EDataType::FLOAT)
KK_FIELD_COUNT(4)
KK_FIELD_RANGE(0.f, 1.f)
KK_FIELD_PUSH(AnimatedDraw, skmodel.material.shininess, EDataType::FLOAT)

KK_FIELD_PUSH(AnimatedDraw, skmodel, EDataType::SKELETON)
KK_FIELD_PUSH(AnimatedDraw, animGraph, EDataType::ANIMATION)
KK_COMPONENT_IMPL_END

void AnimatedDraw::OnStart()
{
    for (Renderer::Socket& socket : skmodel.sockets)
        socket.render = true;
}

void AnimatedDraw::OnUpdate()
{
	animGraph.player.UpdatePlayer(Game::Time::GetDeltaTime());
}

void AnimatedDraw::Draw(Renderer::LowLevel::LowRenderer& i_renderer, const Renderer::Light& i_light, const Game::Transform& i_entityTransform)
{
	if (skmodel.skmesh.expired() || !skmodel.skmesh.lock())
		return;

	Shader& shader = *skmodel.shader;
	shader.Use();
	shader.SetUniform("uMaterial.ambient", skmodel.material.ambient);
	shader.SetUniform("uMaterial.diffuse", skmodel.material.diffuse);
	shader.SetUniform("uMaterial.specular", skmodel.material.specular);
    shader.SetUniform("uMaterial.emissive", skmodel.material.emissive);
    shader.SetUniform("uMaterial.tint", skmodel.material.tint);
	shader.SetUniform("uMaterial.shininess", skmodel.material.shininess);
	shader.SetUniform("uBoneTransforms", MAX_BONES, animGraph.player.GetGlobalBoneMatrices().data()->element);

	{
		GLuint texToBeBinded = ResourcesManager::GetDefaultTexture().ChooseColor(texturePath);

		Resources::Texture* diffuseTex = skmodel.diffuseTex.lock().get();
		if (diffuseTex != nullptr)
			if (diffuseTex->gpu.get())
				texToBeBinded = diffuseTex->gpu->data;

		for (std::shared_ptr<Submesh>& smesh : skmodel.skmesh.lock()->submeshes)
		{
			if (smesh == nullptr || smesh->gpu.VAO == 0)
				continue;

			Matrix4 modelMat = smesh->localTransform * i_entityTransform.GetWorldMatrix();
			shader.SetUniform("uModel", modelMat);
			Matrix4 modelNormal = (modelMat.Inverse()).Transpose();
			shader.SetUniform("uModelNormal", modelNormal);

			assert(smesh->gpu.VAO != 0);
			i_renderer.RenderMeshOnce(smesh->gpu.VAO,
				smesh->vertices.size(),
				texToBeBinded);

			if (i_renderer.outline)
				i_renderer.RenderMeshOnceOutline(smesh->gpu.VAO, smesh->vertices.size());
		}
	}

	for (Renderer::Socket& socket : skmodel.sockets)
	{
        if (!socket.render)
            continue;

		GLuint texToBeBinded = ResourcesManager::GetDefaultTexture().ChooseColor(socket.model.textureName);

		Resources::Texture* diffuseTex = socket.model.diffuseTex.lock().get();
		if (diffuseTex != nullptr)
			if (diffuseTex->gpu.get())
				texToBeBinded = diffuseTex->gpu->data;

		Shader& socketShader = *socket.model.shader;
		socketShader.Use();
		socketShader.SetUniform("uMaterial.ambient", socket.model.material.ambient);
		socketShader.SetUniform("uMaterial.diffuse", socket.model.material.diffuse);
		socketShader.SetUniform("uMaterial.specular", socket.model.material.specular);
        socketShader.SetUniform("uMaterial.emissive", socket.model.material.emissive);
        socketShader.SetUniform("uMaterial.tint", socket.model.material.tint);
		socketShader.SetUniform("uMaterial.shininess", socket.model.material.shininess);

		Matrix4 socketTRS = Matrix4::Scale(socket.transform.scale) *
			Matrix4::RotateXYZ(socket.transform.rotation) *
			Matrix4::Translate(socket.transform.position);
		Matrix4 boneTRS = animGraph.player.GetRawBoneMatrices()[socket.boneID];

		for (std::shared_ptr<Submesh>& smesh : socket.model.mesh.lock()->submeshes)
		{
			if (smesh == nullptr || smesh->gpu.VAO == 0)
				continue;

			Matrix4 modelMat = socketTRS * boneTRS * smesh->localTransform * i_entityTransform.GetWorldMatrix();
			socketShader.SetUniform("uModel", modelMat);
			Matrix4 modelNormal = (modelMat.Inverse()).Transpose();
			socketShader.SetUniform("uModelNormal", modelNormal);

			assert(smesh->gpu.VAO != 0);
			i_renderer.RenderMeshOnce(smesh->gpu.VAO,
				smesh->vertices.size(),
				texToBeBinded);

			if (i_renderer.outline)
				i_renderer.RenderMeshOnceOutline(smesh->gpu.VAO, smesh->vertices.size());
		}
	}

	Resources::Shader::Unuse();
}

void AnimatedDraw::DrawDepthMap(Renderer::LowLevel::LowRenderer& i_renderer, const Game::Transform& i_entityTransform)
{
	if (skmodel.skmesh.expired() || !skmodel.skmesh.lock())
		return;

	Shader& shader = *skmodel.lightDepthShader;
	shader.Use();
	shader.SetUniform("uBoneTransforms",
		MAX_BONES,
		animGraph.player.GetGlobalBoneMatrices().data()->element);

	for (std::shared_ptr<Submesh>& smesh : skmodel.skmesh.lock()->submeshes)
	{
		if (smesh == nullptr || smesh->gpu.VAO == 0)
			continue;

		Matrix4 modelMat = smesh->localTransform * i_entityTransform.GetWorldMatrix();
		shader.SetUniform("uModel", modelMat);
		assert(smesh->gpu.VAO != 0);
		i_renderer.RenderMeshOnce(smesh->gpu.VAO, smesh->vertices.size(), 0);
	}

	for (Renderer::Socket& socket : skmodel.sockets)
	{
        if (!socket.render)
            continue;

		Shader& socketShader = *socket.model.lightDepthShader;
		socketShader.Use();

		Matrix4 socketTRS = Matrix4::Scale(socket.transform.scale) *
			Matrix4::RotateXYZ(socket.transform.rotation) *
			Matrix4::Translate(socket.transform.position);
		Matrix4 boneTRS = animGraph.player.GetRawBoneMatrices()[socket.boneID];

		for (std::shared_ptr<Submesh>& smesh : socket.model.mesh.lock()->submeshes)
		{
			if (smesh == nullptr || smesh->gpu.VAO == 0)
				continue;

			Matrix4 modelMat = socketTRS * boneTRS * smesh->localTransform * i_entityTransform.GetWorldMatrix();
			socketShader.SetUniform("uModel", modelMat);
			assert(smesh->gpu.VAO != 0);
			i_renderer.RenderMeshOnce(smesh->gpu.VAO, smesh->vertices.size(), 0);
		}
	}

	Shader::Unuse();
}

bool AnimatedDraw::IsOpaque() const
{
	if (skmodel.diffuseTex.expired() || skmodel.material.tint.w < 1.f)
		return false; // default textures may be transparent

	return skmodel.diffuseTex.lock()->channels != 4;
}

void AnimatedDraw::SetMesh(const std::string& i_path)
{
	meshPath = i_path;
	skmodel.SetSkeletalMeshFromFile(i_path);
	animGraph.SetOwningSkeletalMesh(skmodel.skmesh.lock());
}

void AnimatedDraw::SetTexture(const std::string& i_path, bool i_flip)
{
	texturePath = i_path;
	skmodel.SetTexture(i_path, i_flip);
}

void AnimatedDraw::SetAnimation(const std::string& i_path)
{
	animGraph.AddAnimationPack(i_path);
}

void DropOnAnimatedDrawComponentMesh(unsigned char* io_component, void* io_dropData)
{
	auto& ad = *reinterpret_cast<AnimatedDraw*>(io_component);
	auto path = *reinterpret_cast<std::filesystem::path*>(io_dropData);
	std::string extension = path.extension().string();
	if (Utils::FindExtension(extension, Utils::MeshExtensions))
		ad.SetMesh(path.string());
}

void DropOnAnimatedDrawComponentTexture(unsigned char* io_component, void* io_dropData)
{
	auto& ad = *reinterpret_cast<AnimatedDraw*>(io_component);
	auto path = *reinterpret_cast<std::filesystem::path*>(io_dropData);
	std::string extension = path.extension().string();
	if (Utils::FindExtension(extension, Utils::TextureExtensions))
		ad.SetTexture(path.string(), false);
}

void OnAnimationMeshPathUpdate(unsigned char* io_component)
{
	auto& sd = *reinterpret_cast<AnimatedDraw*>(io_component);
	sd.SetMesh(sd.meshPath);
}

void OnAnimationTexturePathUpdate(unsigned char* io_component)
{
	auto& sd = *reinterpret_cast<AnimatedDraw*>(io_component);
	sd.SetTexture(sd.texturePath, false);
}
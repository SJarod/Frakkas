#include <cassert>
#include <filesystem>

#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/light.hpp"
#include "resources/resources_manager.hpp"
#include "resources/mesh.hpp"

#include "utils/dragdrop_constants.hpp"

#include "game/entity.hpp"
#include "drawable/static_draw.hpp"

KK_COMPONENT_IMPL_BEGIN(StaticDraw)
KK_FIELD_PUSH(StaticDraw, meshPath, EDataType::STRING)
KK_FIELD_CHANGECALLBACK(OnMeshPathUpdate)
KK_FIELD_DROPTARGET(Utils::ResourcePathDragDropID, DropOnStaticDrawComponent)

KK_FIELD_PUSH(StaticDraw, texturePath, EDataType::STRING)
KK_FIELD_CHANGECALLBACK(OnTexturePathUpdate)
KK_FIELD_DROPTARGET(Utils::ResourcePathDragDropID, DropOnStaticDrawComponent)

KK_FIELD_PUSH(StaticDraw, model.material.ambient, EDataType::FLOAT)
KK_FIELD_COUNT(3)
KK_FIELD_RANGE(0.f, 1.f)
KK_FIELD_PUSH(StaticDraw, model.material.diffuse, EDataType::FLOAT)
KK_FIELD_COUNT(3)
KK_FIELD_RANGE(0.f, 1.f)
KK_FIELD_PUSH(StaticDraw, model.material.specular, EDataType::FLOAT)
KK_FIELD_COUNT(3)
KK_FIELD_RANGE(0.f, 1.f)
KK_FIELD_PUSH(StaticDraw, model.material.emissive, EDataType::FLOAT)
KK_FIELD_COUNT(3)
KK_FIELD_RANGE(0.f, 1.f)
KK_FIELD_PUSH(StaticDraw, model.material.tint, EDataType::FLOAT)
KK_FIELD_COUNT(4)
KK_FIELD_RANGE(0.f, 1.f)
KK_FIELD_PUSH(StaticDraw, model.material.shininess, EDataType::FLOAT)
KK_COMPONENT_IMPL_END

void StaticDraw::Draw(Renderer::LowLevel::LowRenderer& i_renderer, const Renderer::Light& i_light, const Game::Transform& i_entityTransform)
{
	if (model.mesh.expired() || !model.mesh.lock())
		return;

	Shader& shader = *model.shader;

	shader.Use();
	shader.SetUniform("uMaterial.ambient", model.material.ambient);
	shader.SetUniform("uMaterial.diffuse", model.material.diffuse);
	shader.SetUniform("uMaterial.specular", model.material.specular);
    shader.SetUniform("uMaterial.emissive", model.material.emissive);
    shader.SetUniform("uMaterial.tint", model.material.tint);
	shader.SetUniform("uMaterial.shininess", model.material.shininess);

	GLuint texToBeBinded = ResourcesManager::GetDefaultTexture().ChooseColor(texturePath);

	Resources::Texture* diffuseTex = model.diffuseTex.lock().get();
	if (diffuseTex != nullptr)
		if (diffuseTex->gpu.get())
			texToBeBinded = diffuseTex->gpu->data;

	for (std::shared_ptr<Submesh>& smesh : model.mesh.lock()->submeshes)
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
	Resources::Shader::Unuse();
}

void StaticDraw::DrawDepthMap(Renderer::LowLevel::LowRenderer& i_renderer, const Game::Transform& i_entityTransform)
{
	if (model.mesh.expired() || !model.mesh.lock() || !IsOpaque())
		return;

	Shader& shader = *model.lightDepthShader;
	shader.Use();

	for (std::shared_ptr<Submesh>& smesh : model.mesh.lock()->submeshes)
	{
		if (smesh == nullptr || smesh->gpu.VAO == 0)
			continue;

		Matrix4 modelMat = smesh->localTransform * i_entityTransform.GetWorldMatrix();
		shader.SetUniform("uModel", modelMat);
		assert(smesh->gpu.VAO != 0);
		i_renderer.RenderMeshOnce(smesh->gpu.VAO, smesh->vertices.size(), 0);
	}
	Shader::Unuse();
}

bool StaticDraw::IsOpaque() const
{
	if (model.material.tint.w < 1.f)
		return false; // default textures may be transparent
    else if (!model.diffuseTex.expired())
	    return model.diffuseTex.lock()->channels != 4;
    else
        return true;
}

void StaticDraw::SetMesh(const std::string& i_path)
{
	meshPath = i_path;
	model.SetMeshFromFile(meshPath);
}

void StaticDraw::SetTexture(const std::string& i_path, bool i_flip)
{
	texturePath = i_path;
	model.SetTexture(texturePath, i_flip);
}

void DropOnStaticDrawComponent(unsigned char* io_component, void* io_dropData)
{
	auto& sd = *reinterpret_cast<StaticDraw*>(io_component);
	auto path = *reinterpret_cast<std::filesystem::path*>(io_dropData);
	std::string extension = path.extension().string();
	if (Utils::FindExtension(extension, Utils::MeshExtensions))
		sd.SetMesh(path.string());

	if (Utils::FindExtension(extension, Utils::TextureExtensions))
		sd.SetTexture(path.string(), false);
}

void OnMeshPathUpdate(unsigned char* io_component)
{
	auto& sd = *reinterpret_cast<StaticDraw*>(io_component);
	sd.SetMesh(sd.meshPath);
}

void OnTexturePathUpdate(unsigned char* io_component)
{
	auto& sd = *reinterpret_cast<StaticDraw*>(io_component);
	sd.SetTexture(sd.texturePath, false);
}
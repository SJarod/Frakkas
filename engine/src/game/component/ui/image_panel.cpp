#include <filesystem>

#include "utils/dragdrop_constants.hpp"
#include "resources/texture.hpp"
#include "resources/resources_manager.hpp"

#include "ui/image_panel.hpp"

KK_COMPONENT_IMPL_BEGIN(ImagePanel)

    KK_FIELD_PUSH(ImagePanel, imageFlip, EDataType::BOOL)

    KK_FIELD_PUSH(ImagePanel, texturePath, EDataType::STRING)
    KK_FIELD_RENAME("Texture")
    KK_FIELD_CHANGECALLBACK(OnImagePanelTextureSet)
    KK_FIELD_DROPTARGET(Utils::ResourcePathDragDropID, OnImagePanelTextureDrop)

KK_COMPONENT_IMPL_END

void ImagePanel::Render(ImDrawList& io_dl, const Vector2& i_origin, const Vector2& i_windowSize) const
{
    if (texture && texture->loaded.test())
    {
        ImVec2 p1 = {screenPosition.x, screenPosition.y};
        ImVec2 p2 = {screenPosition.x + screenScale.x, screenPosition.y};
        ImVec2 p3 = {screenPosition.x + screenScale.x, screenPosition.y + screenScale.y};
        ImVec2 p4 = {screenPosition.x, screenPosition.y + screenScale.y};

        io_dl.AddImageQuad(reinterpret_cast<ImTextureID>(texture->gpu->data), p1, p2, p3, p4,
                           ImVec2(0, 0),ImVec2(1, 0),ImVec2(1, 1),ImVec2(0, 1),
                           ImGui::ColorConvertFloat4ToU32(ImVec4(tint.x, tint.y, tint.z, tint.w)));
    }
}

void ImagePanel::SetTexture(const std::string& i_texturePath)
{
    texturePath = i_texturePath;
    texture = Resources::ResourcesManager::LoadResource<Resources::Texture>(texturePath, imageFlip);
}

void ImagePanel::SetTexture(const std::shared_ptr<Resources::Texture>& i_texture)
{
    texture = i_texture;
}

void OnImagePanelTextureSet(unsigned char* io_component)
{
    auto image = reinterpret_cast<ImagePanel*>(io_component);
    image->SetTexture(image->texturePath);
}

void OnImagePanelTextureDrop(unsigned char* io_component, void* io_data)
{
    auto image = reinterpret_cast<ImagePanel*>(io_component);
    auto path = reinterpret_cast<std::filesystem::path*>(io_data);
    if (Utils::FindExtension(path->extension().string(), Utils::TextureExtensions))
        image->SetTexture(path->string());
}
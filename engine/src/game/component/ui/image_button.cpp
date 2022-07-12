#include <filesystem>

#include "utils/dragdrop_constants.hpp"
#include "resources/texture.hpp"
#include "resources/resources_manager.hpp"

#include "ui/image_button.hpp"

KK_COMPONENT_IMPL_BEGIN(ImageButton)

    KK_FIELD_PUSH(ImageButton, imageFlip, EDataType::BOOL)

    KK_FIELD_PUSH(ImageButton, texturePath, EDataType::STRING)
    KK_FIELD_RENAME("Texture")
    KK_FIELD_CHANGECALLBACK(OnImageButtonTextureSet)
    KK_FIELD_DROPTARGET(Utils::ResourcePathDragDropID, OnImageButtonTextureDrop)

    KK_FIELD_PUSH(ImageButton, idleColor, EDataType::FLOAT)
    KK_FIELD_RENAME("idle")
    KK_FIELD_COUNT(4)
    KK_FIELD_RANGE(0.f, 1.f)
    KK_FIELD_PUSH(ImageButton, hoveredColor, EDataType::FLOAT)
    KK_FIELD_RENAME("hovered")
    KK_FIELD_COUNT(4)
    KK_FIELD_RANGE(0.f, 1.f)
    KK_FIELD_PUSH(ImageButton, activatedColor, EDataType::FLOAT)
    KK_FIELD_RENAME("activated")
    KK_FIELD_COUNT(4)
    KK_FIELD_RANGE(0.f, 1.f)

KK_COMPONENT_IMPL_END

void ImageButton::AddClickEvent(const ImageButtonClickEvent& i_event)
{
    clickEvent.emplace_back(i_event);
}

void ImageButton::Render(ImDrawList& io_dl, const Vector2& i_origin, const Vector2& i_windowSize) const
{
    if (texture && texture->loaded.test())
    {
        static Vector4 curColor = idleColor;

        ImGui::SetCursorScreenPos(ImVec2(screenPosition.x, screenPosition.y));
        if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(texture->gpu->data), ImVec2(screenScale.x, screenScale.y), ImVec2(0, 0), ImVec2(1, 1),-1, ImVec4(0.f, 0.f, 0.f, 0.f), ImVec4(curColor.x,curColor.y,curColor.z,curColor.w)))
            for (const ImageButtonClickEvent& event : clickEvent)
                event();

        if (ImGui::IsItemClicked())
            curColor = activatedColor;
        else if (ImGui::IsItemHovered())
            curColor = hoveredColor;
        else
            curColor = idleColor;
    }
}

void ImageButton::SetTexture(const std::string& i_texturePath)
{
    texturePath = i_texturePath;
    texture = Resources::ResourcesManager::LoadResource<Resources::Texture>(texturePath, imageFlip);
}

void OnImageButtonTextureSet(unsigned char* io_component)
{
    auto image = reinterpret_cast<ImageButton*>(io_component);
    image->SetTexture(image->texturePath);
}

void OnImageButtonTextureDrop(unsigned char* io_component, void* io_data)
{
    auto image = reinterpret_cast<ImageButton*>(io_component);
    auto path = reinterpret_cast<std::filesystem::path*>(io_data);
    if (Utils::FindExtension(path->extension().string(), Utils::TextureExtensions))
        image->SetTexture(path->string());
}
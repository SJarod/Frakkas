#include "ui/button.hpp"

KK_COMPONENT_IMPL_BEGIN(Button)

    KK_FIELD_PUSH(Button, text, EDataType::STRING)

    KK_FIELD_PUSH(Button, textScale, EDataType::FLOAT)
    KK_FIELD_RANGE(0.1f, 9999.f)

    KK_FIELD_PUSH(Button, idleColor, EDataType::FLOAT)
    KK_FIELD_RENAME("idle")
    KK_FIELD_COUNT(4)
    KK_FIELD_RANGE(0.f, 1.f)
    KK_FIELD_PUSH(Button, hoveredColor, EDataType::FLOAT)
    KK_FIELD_RENAME("hovered")
    KK_FIELD_COUNT(4)
    KK_FIELD_RANGE(0.f, 1.f)
    KK_FIELD_PUSH(Button, activatedColor, EDataType::FLOAT)
    KK_FIELD_RENAME("activated")
    KK_FIELD_COUNT(4)
    KK_FIELD_RANGE(0.f, 1.f)

KK_COMPONENT_END

void Button::AddClickEvent(const ButtonClickEvent& i_event)
{
    clickEvent.emplace_back(i_event);
}

void Button::Render(ImDrawList& io_dl, const Vector2& i_origin, const Vector2& i_windowSize) const
{
    ImGui::SetWindowFontScale(textScale * averageScale);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(idleColor.x, idleColor.y, idleColor.z, idleColor.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4(hoveredColor.x, hoveredColor.y, hoveredColor.z, hoveredColor.w));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,ImVec4(activatedColor.x, activatedColor.y, activatedColor.z, activatedColor.w));

    ImGui::SetCursorScreenPos(ImVec2(screenPosition.x, screenPosition.y));
    if (ImGui::Button(text.empty() ? "." : text.c_str(), ImVec2(screenScale.x, screenScale.y)))
        for (const ButtonClickEvent& event : clickEvent)
            event();
    ImGui::PopStyleColor(3);
}
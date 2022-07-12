#include "ui/panel.hpp"

KK_COMPONENT_IMPL_BEGIN(Panel)
KK_COMPONENT_IMPL_END

void Panel::Render(ImDrawList& io_dl, const Vector2& i_origin, const Vector2& i_windowSize) const
{
    ImVec2 p1 = {screenPosition.x, screenPosition.y};
    ImVec2 p2 = {screenPosition.x + screenScale.x, screenPosition.y};
    ImVec2 p3 = {screenPosition.x, screenPosition.y + screenScale.y};
    ImVec2 p4 = {screenPosition.x + screenScale.x, screenPosition.y + screenScale.y};

    ImU32 col = ImGui::ColorConvertFloat4ToU32(ImVec4(tint.x, tint.y, tint.z, tint.w));
    io_dl.AddQuadFilled(p1, p2, p4, p3, col);
}
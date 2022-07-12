#include "game/lowcomponent/ui_object.hpp"

KK_COMPONENT_IMPL_BEGIN(UIObject)

    KK_FIELD_PUSH_TEXT("Rect transform")

    KK_FIELD_PUSH(UIObject, position, EDataType::FLOAT)
    KK_FIELD_RANGE(-100.f, 100.f)
    KK_FIELD_COUNT(2)
    KK_FIELD_TOOLTIP("position is a percentage")

    KK_FIELD_PUSH(UIObject, scale, EDataType::FLOAT)
    KK_FIELD_RANGE(1.f, 1000.f)
    KK_FIELD_COUNT(2)

    KK_FIELD_PUSH(UIObject, tint, EDataType::FLOAT)
    KK_FIELD_COUNT(4)
    KK_FIELD_RANGE(0.f, 1.f)

    KK_FIELD_PUSH_TEXT("UI parameters")

KK_COMPONENT_IMPL_END

void UIObject::UpdateAndRender(ImDrawList& io_dl, const Vector2& i_origin, const Vector2& i_windowSize)
{
    screenPosition = i_origin + (position / 100.f * i_windowSize);
    float screenSizeAverage = (i_windowSize.x + i_windowSize.y) / 2.f;
    averageScale = (screenSizeAverage / baseWindowScale);
    screenScale = scale * averageScale;

    Render(io_dl, i_origin, i_windowSize);
}
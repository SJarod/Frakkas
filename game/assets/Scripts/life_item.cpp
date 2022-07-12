#include "drawable/static_draw.hpp"
#include "collider/sphere_collider.hpp"

#include "life_item.hpp"

KK_COMPONENT_IMPL_BEGIN(LifeItem)

    KK_FIELD_PUSH(LifeItem, life, EDataType::INT)

KK_COMPONENT_IMPL_END

void LifeItem::OnStart()
{
    startPosition = Position();

    // COLLIDER

    auto trigger = owner.get()->AddComponent<SphereCollider>();
    trigger->SetTrigger();

    // MESH

    auto draw = owner.get()->AddComponent<StaticDraw>();
    draw->SetMesh("game/assets/Models/Heart.fbx");
    draw->SetTexture("red", false);
}

void LifeItem::OnUpdate()
{
    startPosition.y += Maths::Cos(Time::GetTime()) * 2.f;
}


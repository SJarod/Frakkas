#include "player.hpp"

#include "enemy_mesh_rotation.hpp"

KK_COMPONENT_IMPL_BEGIN(EnemyMeshRotation)

KK_COMPONENT_IMPL_END

void EnemyMeshRotation::OnStart()
{}

void EnemyMeshRotation::OnUpdate()
{
    Transform& playerTransform = GetEntityContainer().FindEntityWithComponent<Player>()->transform;

    fromTo = Vector3::VecFromPt(GetRootTransform().position, playerTransform.position).Normalize();
    if (fromTo != Vector3::zero)
        Rotation() = {0.f, Maths::Atan2(-fromTo.x, fromTo.z), 0.f};
}


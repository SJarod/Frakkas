#include "player.hpp"

#include "player_mesh_rotation.hpp"

KK_COMPONENT_IMPL_BEGIN(PlayerMeshRotation)

KK_COMPONENT_IMPL_END

void PlayerMeshRotation::OnUpdate()
{
    auto player = GetEntityContainer().FindEntityWithComponent<Player>()->GetComponent<Player>();

    if (player->XZVelocity != Vector2())
    {
        direction = player->XZVelocity.Normalize();

        Quaternion thisQ = Quaternion::QuatFromEuler(Rotation().get());
    	Quaternion targetQ = Quaternion::QuatFromAxisAngle({ 0.f, 1.f, 0.f }, Maths::Atan2(-direction.x, -direction.y));
    	Vector3 rot = Quaternion::Slerp(thisQ, targetQ, Time::GetDeltaTime() * 10.f).QuatToEuler();

    	Rotation() = rot;
    }
}



#include "game/entity.hpp"
#include "player.hpp"
#include "physic_player.hpp"

#include "ui/button.hpp"
#include "follow_camera.hpp"

using namespace Game;
KK_COMPONENT_IMPL_BEGIN(FollowCamera)
    KK_FIELD_PUSH(FollowCamera, offset, EDataType::FLOAT)
    KK_FIELD_COUNT(3)

    KK_FIELD_PUSH(FollowCamera, lerpFactor, EDataType::FLOAT)
    KK_FIELD_RANGE(0.01f, 1.f)
KK_COMPONENT_IMPL_END

void FollowCamera::OnStart()
{
    if (Entity* player = GetEntityContainer().FindEntityWithComponent<Player>())
        playerTransform = &player->transform;
    else if (Entity* physicPlayer = GetEntityContainer().FindEntityWithComponent<PhysicPlayer>())
        playerTransform = &physicPlayer->transform;

    if (playerTransform)
        Position() = playerTransform->position.get() + offset;

    Button* button = owner.get()->GetComponent<Button>();
    if (button)
        button->AddClickEvent([&](){offset.z += 0.5f;});
}

void FollowCamera::OnUpdate()
{
    if(playerTransform)
    {
        Vector3 targetPosition = offset + playerTransform->position.get();
        Position() = Maths::Lerp(Position().get(), targetPosition, lerpFactor);
    }
}

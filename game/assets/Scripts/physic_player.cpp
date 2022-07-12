#include "pause_trigger.hpp"

#include "physic_player.hpp"

KK_COMPONENT_IMPL_BEGIN(PhysicPlayer)

    KK_FIELD_PUSH(PhysicPlayer, speed, EDataType::FLOAT)
    KK_FIELD_PUSH(PhysicPlayer, jumpForce, EDataType::FLOAT)
    KK_FIELD_PUSH(PhysicPlayer, gravityFactor, EDataType::FLOAT)

KK_COMPONENT_IMPL_END

void PhysicPlayer::OnStart()
{
    rigidBody = owner.get()->GetComponent<SphereCollider>();

    Inputs::SetButtonAction("jump", {EButton::SPACE, EButton::MOUSE_LEFT, EButton::GAMEPAD_A, EButton::GAMEPAD_B});
    Inputs::SetButtonAction("pause", {EButton::ESCAPE, EButton::P, EButton::GAMEPAD_START});
}

void PhysicPlayer::OnUpdate()
{
    if (Inputs::IsPressed("jump"))
        rigidBody->velocity.y = jumpForce * Time::GetFixedDeltaTime();

    if (Inputs::IsPressed("pause"))
    {
        auto pauseTrigger = GetEntityContainer().FindEntityWithComponent<PauseTrigger>();
        pauseTrigger->enabled = !pauseTrigger->enabled;
    }

    float forwardVelocity = Inputs::GetAxis("forward");
    float strafeVelocity = Inputs::GetAxis("horizontal");

    Vector2 gamepadDir = Inputs::GetLeftJoystickDirection();

    float frameSpeed = speed * Time::GetFixedDeltaTime();

    strafeVelocity = Maths::Clamp( strafeVelocity + gamepadDir.x, -1.f, 1.f) * frameSpeed;
    forwardVelocity = Maths::Clamp( forwardVelocity + gamepadDir.y, -1.f, 1.f) * frameSpeed;

    Vector3 translation = Vector3(0.f, rigidBody->velocity.y, 0.f) + Vector3(forwardVelocity, 0.f, strafeVelocity);
    rigidBody->velocity = translation;
}

void OnGravityChanged(unsigned char* io_component)
{
    auto player = reinterpret_cast<PhysicPlayer*>(io_component);
    if (player->rigidBody)
        player->rigidBody->gravityFactor = player->gravityFactor;
}


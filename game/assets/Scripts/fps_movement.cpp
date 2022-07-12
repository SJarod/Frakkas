
#include "maths.hpp"
#include "engine.hpp"

#include "fps_movement.hpp"

KK_COMPONENT_IMPL_BEGIN(FPSMovement)
KK_COMPONENT_IMPL_END

void FPSMovement::OnUpdate()
{
    float frameSpeed = 20.f * Time::GetDeltaTime();

    float forwardVelocity = Inputs::GetAxis("forward") * frameSpeed;
    float strafeVelocity = Inputs::GetAxis("horizontal") * frameSpeed;

    Vector3 pos = owner.get()->transform.position;
    Vector3 rot = owner.get()->transform.rotation;

    pos.x += Maths::Sin(rot.y) * forwardVelocity + Maths::Cos(rot.y) * strafeVelocity;
    pos.z += Maths::Sin(rot.y) * strafeVelocity - Maths::Cos(rot.y) * forwardVelocity;

    rot.y += Inputs::GetMouseDelta().x * 0.005f;
    rot.x -= Inputs::GetMouseDelta().y * 0.005f;

    rot.y = Maths::Modulo(rot.y, Maths::Constants::doublePi);
    rot.x = Maths::Clamp(rot.x, -Maths::Constants::halfPi, Maths::Constants::halfPi);

    owner.get()->transform.position = pos;
    owner.get()->transform.rotation = rot;
}

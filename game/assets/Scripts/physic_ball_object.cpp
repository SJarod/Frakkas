#include "physic_ball_object.hpp"

KK_COMPONENT_IMPL_BEGIN(PhysicBallObject)

KK_COMPONENT_IMPL_END

void PhysicBallObject::OnStart()
{
    startPosition = Position();
    mesh = owner.get()->GetComponent<StaticDraw>();
}

void PhysicBallObject::OnUpdate()
{
    if(restart)
    {
        multicolor = true;
        Position() = startPosition;
        restart = false;
    }
    if (multicolor)
    {
        mesh->model.material.tint = Vector4(Maths::Cos(Time::GetTime() * 1.5f),
                                            Maths::Cos(Time::GetTime() * 1.25f),
                                            Maths::Cos(Time::GetTime()),
                                            1.f);
    }
}


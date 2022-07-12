#include "collider/sphere_collider.hpp"
#include "drawable/static_draw.hpp"
#include "physic_player.hpp"

#include "physic_ball_trigger.hpp"

KK_COMPONENT_IMPL_BEGIN(PhysicBallTrigger)

KK_COMPONENT_IMPL_END

void PhysicBallTrigger::OnStart()
{
    Scale() = {0.5f, 0.5f, 0.5f};
}

void PhysicBallTrigger::OnUpdate()
{
    if (createBall)
    {
        ball = GetEntityContainer().CreateEntity("ball");
        Collider* collider = ball->AddComponent<SphereCollider>();
        collider->SetDynamic();
        auto draw = ball->AddComponent<StaticDraw>();
        draw->SetMesh("ProceduralSphere");
        draw->SetTexture("black", false);
        ball->AddComponent<PhysicBallObject>();

        ball->transform.position = Position().get() + Vector3::up * 5.f;
        ball->transform.scale = {maxScale, maxScale, maxScale};

        owner.get()->destroy = true;
    }
}

void PhysicBallTrigger::OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider)
{
}

void PhysicBallTrigger::OnTriggerStay(const Collider& i_ownerCollider, const Collider& i_otherCollider)
{
    if (auto player = i_otherCollider.owner.get()->GetComponent<PhysicPlayer>())
    {
        Scale() = Scale().get() + Vector3(1.f, 1.f, 1.f) * Time::GetDeltaTime() * 0.5f;

        if (Scale().get().x >= maxScale)
            createBall = true;
    }
}

void PhysicBallTrigger::OnTriggerExit(const Collider& i_ownerCollider, const Collider& i_otherCollider)
{
    if (auto player = i_otherCollider.owner.get()->GetComponent<PhysicPlayer>())
    {
        Scale() = {0.5f, 0.5f, 0.5f};
    }
}


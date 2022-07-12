#include "physic_ball_object.hpp"
#include "collider/sphere_collider.hpp"

#include "physic_goal_trigger.hpp"

KK_COMPONENT_IMPL_BEGIN(PhysicGoalTrigger)

KK_COMPONENT_IMPL_END

void PhysicGoalTrigger::OnStart()
{}

void PhysicGoalTrigger::OnUpdate()
{

}

void PhysicGoalTrigger::OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider)
{
    if (auto ball = i_otherCollider.owner.get()->GetComponent<PhysicBallObject>())
        ball->restart = true;
}


#include "debug/log.hpp"
#include "game/lowcomponent/collider.hpp"
#include "physic/physic_scene.hpp"

#include "physic/contact_listener.hpp"

JPH::ValidateResult	JPH::MyContactListener::OnContactValidate(const Body& i_body1, const Body& i_body2, const CollideShapeResult& i_collisionResult)
{
    if (i_body1.GetObjectLayer() == JPH::Layers::DISABLE || i_body2.GetObjectLayer() == JPH::Layers::DISABLE)
        return ValidateResult::RejectContact;
    else
	    return ValidateResult::AcceptContact;
}

// OnCollisionEnter
void JPH::MyContactListener::OnContactAdded(const Body& i_body1, const Body& i_body2, const ContactManifold& i_manifold, ContactSettings& io_settings)
{
	if (i_body1.IsSensor() || i_body2.IsSensor())
        Physic::PhysicScene::NotifyCollision(Physic::ECollisionEvent::TRIGGER_ENTER, i_body1.GetID(),i_body2.GetID());
    else
        Physic::PhysicScene::NotifyCollision(Physic::ECollisionEvent::COLLISION_ENTER, i_body1.GetID(),i_body2.GetID());
}

// OnCollisionStay
void JPH::MyContactListener::OnContactPersisted(const Body& i_body1, const Body& i_body2, const ContactManifold& i_manifold, ContactSettings& io_settings)
{
	if (i_body1.IsSensor() || i_body2.IsSensor())
        Physic::PhysicScene::NotifyCollision(Physic::ECollisionEvent::TRIGGER_STAY, i_body1.GetID(),i_body2.GetID());
	else
		Physic::PhysicScene::NotifyCollision(Physic::ECollisionEvent::COLLISION_STAY, i_body1.GetID(),i_body2.GetID());
}

// OnColllisionExit
void JPH::MyContactListener::OnContactRemoved(const SubShapeIDPair& i_subShapePair)
{
    auto colliders = Physic::PhysicScene::colliders;
    auto it1 =std::find_if(colliders.begin(), colliders.end(), [&i_subShapePair](const Game::Collider* collider)
    {
        return i_subShapePair.GetBody1ID() == collider->GetPhysicBodyID();
    });
    auto it2 =std::find_if(colliders.begin(), colliders.end(), [&i_subShapePair](const Game::Collider* collider)
    {
        return i_subShapePair.GetBody2ID() == collider->GetPhysicBodyID();
    });
    // if it does not find a collider, cancel the remove
    if (it1 != colliders.end() && it2 != colliders.end())
	    Physic::PhysicScene::NotifyCollisionExit(i_subShapePair.GetBody1ID(), i_subShapePair.GetBody2ID());
}
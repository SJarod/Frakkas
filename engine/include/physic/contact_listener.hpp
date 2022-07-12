#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ContactListener.h>

JPH_NAMESPACE_BEGIN

class MyContactListener : public ContactListener
{
	ValidateResult	OnContactValidate(const Body& i_body1, const Body& i_body2, const CollideShapeResult& i_collisionResult) override;

	void OnContactAdded(const Body& i_body1, const Body& i_body2, const ContactManifold& i_manifold, ContactSettings& io_settings) override;

	void OnContactPersisted(const Body& i_body1, const Body& i_body2, const ContactManifold& i_manifold, ContactSettings& io_settings) override;

	void OnContactRemoved(const SubShapeIDPair& i_subShapePair) override;
};

JPH_NAMESPACE_END
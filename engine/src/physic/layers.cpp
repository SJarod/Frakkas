#include "physic/layers.hpp"

bool JPH::MyObjectCanCollide(ObjectLayer i_object1, ObjectLayer i_object2)
{
    switch (i_object1)
    {
        case Layers::NON_MOVING: // Collide only with moving
            return i_object2 == Layers::MOVING;
        case Layers::MOVING:  // Collide with everything
        case Layers::SENSOR:
            return true;
        case Layers::DISABLE: // Collide with nothing
            return false;
        default:
            JPH_ASSERT(false);
            return false;
    }
}

bool JPH::MyBroadPhaseCanCollide(ObjectLayer i_layer1, BroadPhaseLayer i_layer2)
{
    switch (i_layer1)
    {
        case Layers::NON_MOVING:
            return i_layer2 == BroadPhaseLayers::MOVING;
        case Layers::MOVING:
        case Layers::SENSOR:
            return true;
        case Layers::DISABLE:
            return false;
        default:
            JPH_ASSERT(false);
            return false;
    }
}

JPH::MyBroadPhaseLayerInterface::MyBroadPhaseLayerInterface()
{
    objectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
    objectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
    objectToBroadPhase[Layers::SENSOR] = BroadPhaseLayers::SENSOR;
    objectToBroadPhase[Layers::DISABLE] = BroadPhaseLayers::DISABLE;
}

JPH::uint JPH::MyBroadPhaseLayerInterface::GetNumBroadPhaseLayers() const
{
    return BroadPhaseLayers::NUM_LAYERS;
}

JPH::BroadPhaseLayer JPH::MyBroadPhaseLayerInterface::GetBroadPhaseLayer(ObjectLayer i_layer) const
{
    JPH_ASSERT(i_layer < Layers::NUM_LAYERS);
    return objectToBroadPhase[i_layer];
}

#ifdef JPH_PROFILE_ENABLED
const char *			JPH::MyBroadPhaseLayerInterface::GetBroadPhaseLayerName(BroadPhaseLayer inLayer) const
{
    switch ((BroadPhaseLayer::Type)inLayer)
    {
    case (BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:	return "NON_MOVING";
    case (BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:		return "MOVING";
    case (BroadPhaseLayer::Type)BroadPhaseLayers::SENSOR:		return "SENSOR";
    case (BroadPhaseLayer::Type)BroadPhaseLayers::DISABLE:		return "DISABLE";
    default:													JPH_ASSERT(false); return "INVALID";
    }
}
#endif
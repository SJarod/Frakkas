#include <Jolt/Jolt.h>

#include <Jolt/Physics/Body/Body.h>

#include "game/transform.hpp"
#include "game/entity.hpp"

#include "game/lowcomponent/collider.hpp"

KK_COMPONENT_IMPL_BEGIN(Collider)

    KK_FIELD_PUSH(Collider, cTrigger, EDataType::BOOL)
    KK_FIELD_CHANGECALLBACK(OnColliderTriggerChanged)
    KK_FIELD_PUSH(Collider, cStatic, EDataType::BOOL)
    KK_FIELD_CHANGECALLBACK(OnColliderStaticChanged)
    KK_FIELD_PUSH(Collider, cKinematic, EDataType::BOOL)
    KK_FIELD_CHANGECALLBACK(OnColliderKinematicChanged)
    KK_FIELD_PUSH(Collider, cDynamic, EDataType::BOOL)
    KK_FIELD_CHANGECALLBACK(OnColliderDynamicChanged)

    KK_FIELD_PUSH_TEXT("Constraints:")
    KK_FIELD_TOOLTIP("Prevent update for selected coordinates.")

    KK_FIELD_PUSH(Collider, constraintPositionX, EDataType::BOOL) KK_FIELD_RENAME("posX")
    KK_FIELD_PUSH(Collider, constraintPositionY, EDataType::BOOL) KK_FIELD_RENAME("posY")
    KK_FIELD_SAMELINE
    KK_FIELD_PUSH(Collider, constraintPositionZ, EDataType::BOOL) KK_FIELD_RENAME("posZ")
    KK_FIELD_SAMELINE

    KK_FIELD_PUSH(Collider, constraintRotationX, EDataType::BOOL) KK_FIELD_RENAME("rotX")
    KK_FIELD_PUSH(Collider, constraintRotationY, EDataType::BOOL) KK_FIELD_RENAME("rotY")
    KK_FIELD_SAMELINE
    KK_FIELD_PUSH(Collider, constraintRotationZ, EDataType::BOOL) KK_FIELD_RENAME("rotZ")
    KK_FIELD_SAMELINE

    KK_FIELD_PUSH(Collider, velocity, EDataType::FLOAT)
    KK_FIELD_COUNT(3)
    KK_FIELD_VIEWONLY

    KK_FIELD_PUSH(Collider, angularVelocity, EDataType::FLOAT)
    KK_FIELD_COUNT(3)
    KK_FIELD_VIEWONLY

KK_COMPONENT_IMPL_END

void Collider::OnEnable()
{
    if (cTrigger)
        SetTrigger();
    else if (cStatic)
        SetStatic();
    else if (cKinematic)
        SetKinematic();
    else if (cDynamic)
        SetDynamic();
}

void Collider::OnDisable()
{
    bodyInterface->SetObjectLayer(GetPhysicBodyID(), JPH::Layers::DISABLE);
}

void Collider::ApplyPhysicTransform()
{
    JPH::Vec3 pos = bodyInterface->GetPosition(GetPhysicBodyID());
    Position() = {
            constraintPositionX ? Position().get().x : pos.GetX(),
            constraintPositionY ? Position().get().y : pos.GetY(),
            constraintPositionZ ? Position().get().z : pos.GetZ()
    };

    JPH::Quat rot = bodyInterface->GetRotation(GetPhysicBodyID());
    Vector3 eulerRotation = Quaternion(-rot.GetX(), -rot.GetY(), -rot.GetZ(), rot.GetW()).QuatToEuler();

    eulerDiff = Vector3::zero;
    Vector3 newRot = Rotation();
    if (!constraintRotationX)
        newRot.x = eulerRotation.x;
    else
        eulerDiff.x = eulerRotation.x - Rotation().get().x;

    if (!constraintRotationY)
        newRot.y = eulerRotation.y;
    else
        eulerDiff.y = eulerRotation.y - Rotation().get().y;

    if (!constraintRotationZ)
        newRot.z = eulerRotation.z;
    else
        eulerDiff.z = eulerRotation.z - Rotation().get().z;

    Rotation() = newRot;
}

void Collider::ApplyPhysicVelocity()
{
    JPH::Vec3 vel = bodyInterface->GetLinearVelocity(GetPhysicBodyID());
    velocity = Vector3(vel.GetX(), vel.GetY(), vel.GetZ());
}

void Collider::ApplyPhysicAngularVelocity()
{
    JPH::Vec3 vel = bodyInterface->GetAngularVelocity(GetPhysicBodyID());
    angularVelocity = {vel.GetX(), vel.GetY(), vel.GetZ()};
}

void Collider::ApplyEntityTransform()
{
    Matrix4 world = GetTransform().GetWorldMatrix();
    world = world.OrthoNormalize();

    /// Global Position
    Vector3 p = world.DecomposeTranslation();
    bodyInterface->SetPosition(GetPhysicBodyID(), { p.x, p.y, p.z}, JPH::EActivation::Activate);

    /// Rotation
    Quaternion q = Quaternion::QuatFromMatrix(world).Normalize() * Quaternion::QuatFromEuler(eulerDiff);
    bodyInterface->SetRotation(GetPhysicBodyID(), {-q.x, -q.y , -q.z, q.w}, JPH::EActivation::Activate);
}

void Collider::SetPhysicParameters(JPH::Body* i_body, JPH::BodyInterface* i_bodyInterface)
{
    body = i_body;
    bodyInterface =  i_bodyInterface;
}

void Collider::SetTrigger()
{
    cTrigger = true;
    cStatic = false;
    cKinematic = false;
    cDynamic = false;

    bodyInterface->SetMotionType(GetPhysicBodyID(), JPH::EMotionType::Static, JPH::EActivation::Activate);
    bodyInterface->SetObjectLayer(GetPhysicBodyID(), JPH::Layers::SENSOR);
    body->SetIsSensor(cTrigger);
}

void Collider::SetStatic()
{
    cTrigger = false;
    cStatic = true;
    cKinematic = false;
    cDynamic = false;

    bodyInterface->SetMotionType(GetPhysicBodyID(), JPH::EMotionType::Static, JPH::EActivation::DontActivate);
    bodyInterface->SetObjectLayer(GetPhysicBodyID(), JPH::Layers::NON_MOVING);
    body->SetIsSensor(cTrigger);
}

void Collider::SetKinematic()
{
    cTrigger = false;
    cStatic = false;
    cKinematic = true;
    cDynamic = false;

    bodyInterface->SetMotionType(GetPhysicBodyID(), JPH::EMotionType::Kinematic, JPH::EActivation::Activate);
    bodyInterface->SetObjectLayer(GetPhysicBodyID(), JPH::Layers::MOVING);
    body->SetIsSensor(cTrigger);
}

void Collider::SetDynamic()
{
    cTrigger = false;
    cStatic = false;
    cKinematic = false;
    cDynamic = true;

    bodyInterface->SetMotionType(GetPhysicBodyID(), JPH::EMotionType::Dynamic, JPH::EActivation::Activate);
    bodyInterface->SetObjectLayer(GetPhysicBodyID(), JPH::Layers::MOVING);
    body->SetIsSensor(cTrigger);
}

bool Collider::IsTrigger() const
{
    return cTrigger;
}

bool Collider::IsStatic() const
{
    return cStatic;
}

bool Collider::IsKinematic() const
{
    return cKinematic;
}

bool Collider::IsDynamic() const
{
    return cDynamic;
}

void Collider::ApplyEntityVelocity()
{
    JPH::Vec3 vel = { velocity.x,velocity.y, velocity.z };
    bodyInterface->SetLinearVelocity(GetPhysicBodyID(), vel);
}

void Collider::ApplyEntityAngularVelocity()
{
    JPH::Vec3 vel = { angularVelocity.x,angularVelocity.y, angularVelocity.z };
    bodyInterface->SetAngularVelocity(GetPhysicBodyID(), vel);
}

void Collider::ApplyEntityUpdate()
{
    ApplyEntityTransform();
    ApplyEntityVelocity();
    ApplyEntityAngularVelocity();
    bodyInterface->SetGravityFactor(GetPhysicBodyID(), gravityFactor);
}

void Collider::ApplyPhysicUpdate()
{
    ApplyPhysicTransform();
    ApplyPhysicVelocity();
    ApplyPhysicAngularVelocity();
}

const JPH::BodyID& Collider::GetPhysicBodyID() const
{
    return body->GetID();
}

void OnColliderTriggerChanged(unsigned char* io_component)
{
    auto collider = reinterpret_cast<Collider*>(io_component);
    if (collider->IsTrigger())
        collider->SetTrigger();
}

void OnColliderStaticChanged(unsigned char* io_component)
{
    auto collider = reinterpret_cast<Collider*>(io_component);
    if (collider->IsStatic())
        collider->SetStatic();
}

void OnColliderKinematicChanged(unsigned char* io_component)
{
    auto collider = reinterpret_cast<Collider*>(io_component);
    if (collider->IsKinematic())
        collider->SetKinematic();
}

void OnColliderDynamicChanged(unsigned char* io_component)
{
    auto collider = reinterpret_cast<Collider*>(io_component);
    if (collider->IsDynamic())
        collider->SetDynamic();
}
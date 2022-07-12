#include <imgui.h>

#include "game/entity.hpp"
#include "camera.hpp"

KK_COMPONENT_IMPL_BEGIN(Camera)
    KK_FIELD_PUSH(Camera, near, EDataType::FLOAT)
    KK_FIELD_RANGE(0.001f, 10.f)
    KK_FIELD_PUSH(Camera, far, EDataType::FLOAT)
    KK_FIELD_RANGE(100.f, 5000.f)
    KK_FIELD_PUSH(Camera, fovY, EDataType::FLOAT)
    KK_FIELD_RANGE(0.0f, 180.0f)
KK_COMPONENT_IMPL_END

Matrix4 Camera::GetViewMatrix() const
{
    Vector3 pos = owner.get()->transform.position, rot = owner.get()->transform.rotation;

    Vector3 invertRot = { rot.x, -rot.y, rot.z };
    return Matrix4::Translate(-pos) * Matrix4::RotateXYZ(invertRot);
}

Matrix4 Camera::GetProjectionMatrix(float i_aspectRatio) const
{
    return Matrix4::Perspective(Maths::ToRadians(fovY), i_aspectRatio, near, far, false);
}

Matrix4 Camera::GetCascadeProjectionMatrix(float i_aspectRatio, float i_near, float i_far) const
{
    return Matrix4::Perspective(Maths::ToRadians(fovY), i_aspectRatio, i_near, i_far, false);
}
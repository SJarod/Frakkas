#include "game/transform.hpp"

using namespace Game;

Transform::Transform()
{
    position = Vector3::zero;
    rotation = Vector3::zero;
    scale = Vector3(1.f, 1.f, 1.f);
    position.setter = [&](const Vector3& value)
    {
        if (position != value)
            needUpdate = needUniformUpdate = true;
        position.set(value);
    };
    rotation.setter = [&](const Vector3& value)
    {
        if (rotation != value)
            needUpdate = needUniformUpdate = true;
        rotation.set(value);
    };
    scale.setter = [&](const Vector3& value)
    {
        if (scale != value)
            needUpdate = needUniformUpdate = true;
        scale.set(value);
        scaleLockParams.origScale = scale.get();
        scaleLockParams.ratio = 1.f;
    };
    parent.setter = [&](Transform* value)
    {
        // Check if this transform, is not a parent of the input transform 'value'
        if (value)
        {
            bool isParent = false;
            Transform* p = value->parent;
            while (p)
            {
                if (this == p) isParent = true;
                p = p->parent;
            }
            if (isParent) return;
        }

        parent.set(value);
        needUpdate = needUniformUpdate = true;
        if (value)
            value->childs.emplace_back(this);
    };
    parent = nullptr;
}

Transform::Transform(const Transform& i_copy)
{
    position = i_copy.position;
    rotation = i_copy.rotation;
    scale = i_copy.scale;
    parent = i_copy.parent;
    colliderComponentCount = i_copy.colliderComponentCount;
    needUpdate = i_copy.needUpdate;
    needUniformUpdate = i_copy.needUniformUpdate;
    scaleLockParams = i_copy.scaleLockParams;
    worldMatrix = i_copy.worldMatrix;
    childs = i_copy.childs;

    position.setter = [&](const Vector3& value)
    {
        if (position != value)
            needUpdate = needUniformUpdate = true;
        position.set(value);
    };
    rotation.setter = [&](const Vector3& value)
    {
        if (rotation != value)
            needUpdate = needUniformUpdate = true;
        rotation.set(value);
    };
    scale.setter = [&](const Vector3& value)
    {
        if (scale != value)
            needUpdate = needUniformUpdate = true;
        scale.set(value);
        scaleLockParams.origScale = scale.get();
        scaleLockParams.ratio = 1.f;
    };
    parent.setter = [&](Transform* value)
    {
        // Check if this transform, is not a parent of the input transform 'value'
        if (value)
        {
            bool isParent = false;
            Transform* p = value->parent;
            while (p)
            {
                if (this == p) isParent = true;
                p = p->parent;
            }
            if (isParent) return;
        }

        parent.set(value);
        needUpdate = needUniformUpdate = true;
        if (value)
            value->childs.emplace_back(this);
    };
}

Matrix4 Transform::GetWorldMatrix() const
{
    bool parentUpdate = (parent.get() && parent.get()->needUpdate);

    if (needUpdate || parentUpdate)
        UpdateWorldMatrix();

    return worldMatrix;
}

void Transform::RemoveChild(Transform* childToRemove)
{
    std::list<Transform*>::iterator it;
    it = std::find(childs.begin(), childs.end(), childToRemove);

    if (it != childs.end())
        childs.erase(it);
}

void Transform::UpdateWorldMatrix() const
{
    Matrix4 localMatrix = Matrix4::Scale(scale) * Matrix4::RotateXYZ(rotation) * Matrix4::Translate(position);

    if (parent)
        worldMatrix = localMatrix * parent.get()->GetWorldMatrix();
    else
        worldMatrix = localMatrix;

    needUpdate = false;
    for (Transform* child: childs)
        child->needUpdate = true;
}

void Transform::ClearChilds()
{
    for (Transform* child : childs)
        child->parent = nullptr;

    childs.clear();
}

Quaternion Transform::GetQuaternionRotation() const
{
    return Quaternion::QuatFromEuler(-rotation.get());
}

#include <Jolt/Jolt.h>

#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyInterface.h>

#include "resources/mesh.hpp"

#include "renderer/lowlevel/lowrenderer.hpp"
#include "renderer/model.hpp"

#include "collider/sphere_collider.hpp"

KK_COMPONENT_IMPL_BEGIN(SphereCollider)
KK_COMPONENT_IMPL_END

void SphereCollider::UpdateSphereShape()
{
    const Vector3& scale = GetTransform().GetWorldMatrix().DecomposeScale(); // WORLD SCALE

    float radius = Maths::Max(scale.x, scale.y, scale.z);
    if (radius == 0.f || radius == prevRadius)
        return;

    auto* sphere = new JPH::SphereShape(radius);
    bodyInterface->SetShape(GetPhysicBodyID(), sphere, false, JPH::EActivation::Activate);

    prevRadius = radius;
}

void SphereCollider::ApplyEntityUpdate()
{
    Collider::ApplyEntityUpdate();

    UpdateSphereShape();
}

void SphereCollider::DebugDraw(LowRenderer& i_renderer, const Game::Transform& i_entityTransform) const
{
    if (!debugModel.mesh.lock())
        debugModel.SetMeshFromFile(Resources::Mesh::sphereColliderMesh);

    for (auto& smesh : debugModel.mesh.lock()->submeshes)
    {
        Matrix4 modelMat = smesh->localTransform *  i_entityTransform.GetWorldMatrix();

        i_renderer.RenderLines(smesh->gpu.VAO, smesh->vertices.size(), modelMat, 5, {0.5f, 1.f, 0.5f}, false);
    }
}
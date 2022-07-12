#include <Jolt/Jolt.h>

#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Body/BodyInterface.h>

#include "resources/mesh.hpp"
#include "renderer/lowlevel/lowrenderer.hpp"

#include "collider/box_collider.hpp"

KK_COMPONENT_IMPL_BEGIN(BoxCollider)
KK_COMPONENT_IMPL_END

void BoxCollider::UpdateBoxScale()
{
    Vector3 extension = GetTransform().GetWorldMatrix().DecomposeScale(); // World scale

    if (extension.x < 0.05f || extension.y < 0.05f || extension.z < 0.05f || extension == prevExtension)
        return;

    auto* newBox = new JPH::BoxShape(JPH::Vec3(extension.x, extension.y, extension.z));
    bodyInterface->SetShape(GetPhysicBodyID(), newBox, false, JPH::EActivation::Activate);

    prevExtension = extension;
}

void BoxCollider::ApplyEntityUpdate()
{
    Collider::ApplyEntityUpdate();

    UpdateBoxScale();
}

void BoxCollider::DebugDraw(LowRenderer& i_renderer, const Game::Transform& i_entityTransform) const
{
    if (!debugModel.mesh.lock())
        debugModel.SetMeshFromFile(Resources::Mesh::cubeColliderMesh);

    for (auto& smesh : debugModel.mesh.lock()->submeshes)
    {
        Matrix4 modelMat = smesh->localTransform *  i_entityTransform.GetWorldMatrix();

        i_renderer.RenderLines(smesh->gpu.VAO, smesh->vertices.size(), modelMat, 5, {0.5f, 1.f, 0.5f}, true);
    }
}
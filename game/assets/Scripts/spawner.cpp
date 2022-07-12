#include <random>

#include "utils/dragdrop_constants.hpp"
#include "world_data.hpp"

#include "golem.hpp"
#include "skeleton.hpp"

#include "spawner.hpp"

KK_COMPONENT_IMPL_BEGIN(Spawner)

    KK_FIELD_PUSH(Spawner, spawnTime, EDataType::FLOAT)

    KK_FIELD_PUSH(Spawner, skeletonRatio, EDataType::FLOAT)
    KK_FIELD_PUSH(Spawner, golemRatio, EDataType::FLOAT)
    KK_FIELD_PUSH(Spawner, nothingRatio, EDataType::FLOAT)

KK_COMPONENT_IMPL_END

void Spawner::OnStart()
{
    owner.get()->name = "Spawner_" + owner.get()->GetStringID();

    rigidBody = owner.get()->AddComponent<SphereCollider>();
    rigidBody->SetTrigger();

    skeleton = GetEntityContainer().FindEntityWithComponent<Skeleton>();
    golem = GetEntityContainer().FindEntityWithComponent<Golem>();

    // Increase spawn time speed, 1s minimum
    if (auto worldData = World::GetWorldData<FrakkarenaWorldData>())
        spawnTime = Maths::Max(spawnTime * (1.f - worldData->level * 0.1f), 1.f);
}

void Spawner::OnUpdate()
{
    curSpawnTime += Time::GetDeltaTime();
    if (curSpawnTime >= spawnTime && canSpawn)
    {
        Spawn();
        curSpawnTime = 0.f;
    }
}

void Spawner::OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider)
{
    if (auto player = i_otherCollider.owner.get()->GetRootEntity()->GetComponent<Player>())
        canSpawn = false;
}

void Spawner::OnTriggerExit(const Collider& i_ownerCollider, const Collider& i_otherCollider)
{
    if (auto player = i_otherCollider.owner.get()->GetRootEntity()->GetComponent<Player>())
        canSpawn = true;
}

void Spawner::Spawn()
{
    std::random_device randDevice;
    std::mt19937_64 gen(randDevice());
    std::discrete_distribution<> discreteDistri({skeletonRatio, golemRatio, nothingRatio});

    if (discreteDistri(gen) == 0 && skeleton)
    {
        Entity* en = GetEntityContainer().CloneEntity(*skeleton);
        en->enabled = false; // Disable
        en->enabled = true; // Then enable to trigger the callback
        en->transform.position = Position().get();
    }
    else if (discreteDistri(gen) == 1 && golem)
    {
        Entity* en = GetEntityContainer().CloneEntity(*golem);
        en->enabled = false; // Disable
        en->enabled = true; // Then enable to trigger the callback
        en->transform.position = Position().get();
    }
}

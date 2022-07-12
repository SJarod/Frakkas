#pragma once

#include "collider/sphere_collider.hpp"

#include "component_generator.hpp"

namespace Game
{
    KK_COMPONENT(Spawner);

    private:
        SphereCollider* rigidBody = nullptr;

        bool canSpawn = true;
        float curSpawnTime = 0.f;
        float spawnTime = 5.f;

        Entity* skeleton = nullptr;
        float skeletonRatio = 75.f;

        Entity* golem = nullptr;
        float golemRatio = 15.f;

        float nothingRatio = 15.f;

        void OnStart() override;

        void OnUpdate() override;

        void OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider) override;

        void OnTriggerExit(const Collider& i_ownerCollider, const Collider& i_otherCollider) override;

        void Spawn();

    KK_COMPONENT_END
}

void OnSkeletonDrop(unsigned char* io_component, void* io_dropData);
void OnSkeletonNameChanged(unsigned char* io_component);

void OnGolemDrop(unsigned char* io_component, void* io_dropData);
void OnGolemNameChanged(unsigned char* io_component);
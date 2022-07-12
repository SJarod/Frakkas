#pragma once

#include "game/transform.hpp"
#include "collider/sphere_collider.hpp"
#include "collider/box_collider.hpp"
#include "drawable/animated_draw.hpp"
#include "sound.hpp"

#include "enemy_stats.hpp"
#include "life.hpp"
#include "enemy_mesh_rotation.hpp"

#include "component_generator.hpp"

namespace Game
{
    KK_COMPONENT(Skeleton);

public:
        EnemyStats* stats = nullptr;

private:
        bool pause = false;

        SphereCollider* rigidBody = nullptr;
        Transform* playerTransform = nullptr;
        AnimatedDraw* animDraw = nullptr;
        BoxCollider* triggerCollider = nullptr;
        BoxCollider* weaponCollider = nullptr;
        EnemyMeshRotation* enemyMeshRotation = nullptr;

        Life* life = nullptr;

        Sound* walkSound = nullptr;
        Sound* attackSound = nullptr;
        Sound* hitSound = nullptr;
        Sound* deathSound = nullptr;

        Vector3 translation = Vector3::zero;
        Vector3 vecDir = Vector3::zero;

        float distFromPlayer = 0.f;

        float curThinkingTime = 0.f;

        float curChargingTime = 0.f;
        float chargingTime = 0.4f;

        float curAttackTime = 0.f;
        float attackTime = 2.f;
        float attackRange = 15.f;

        float hurtEffectTime = 0.1f;
        float curHurtEffectTime = 0.f;
        Vector4 hurtEffectTint = Vector4(0.5f, 0.5f, 0.f, 1.f);

        float deathTime = 2.f;
        float curDeathTime = 0.f;

        void OnStart() override;

        void OnUpdate() override;

        void OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider) override;


    private:
        void GoToRun();
        void GoToThink();
        void GoToCharge();
        void GoToAttack();
        void GoToSuffer();
        void GoToDeath();

        void Run();
        void Think();
        void Charge();
        void Attack();
        void Suffer();
        void Death();

        void StopSounds();
        void SetSounds(Entity* i_soundChild);

    KK_COMPONENT_END
}

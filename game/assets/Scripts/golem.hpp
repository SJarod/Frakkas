#pragma once

#include "game/transform.hpp"
#include "collider/sphere_collider.hpp"
#include "collider/box_collider.hpp"
#include "drawable/animated_draw.hpp"
#include "drawable/static_draw.hpp"
#include "sound.hpp"
#include "enemy_mesh_rotation.hpp"
#include "player.hpp"
#include "life.hpp"

#include "enemy_stats.hpp"
#include "component_generator.hpp"


namespace Game
{
    KK_COMPONENT(Golem);

        EnemyStats* stats = nullptr;
    private:
        SphereCollider* rigidBody = nullptr;
        Transform* playerTransform = nullptr;
        AnimatedDraw* animDraw = nullptr;
        BoxCollider* triggerCollider = nullptr;
        Player* player = nullptr;
        Life* life = nullptr;
        EnemyMeshRotation* enemyMeshRotation = nullptr;
        StaticDraw* attackDraw = nullptr;

        Sound* walkSound = nullptr;
        Sound* roarSound = nullptr;
        Sound* attackSound = nullptr;
        Sound* hitSound = nullptr;
        Sound* deathSound = nullptr;

        Vector3 translation = Vector3::zero;
        Vector3 vecDir = Vector3::zero;

        float distFromPlayer = 0.f;

        float curThinkingTime = 0.f;

        float chargeTime = 1.4f; // Animation duration = ~3.57s -> 214 ticks (60 ticks/s) // mid = 1.785 // feet on ground ~1.40
        float curChargeTime = 0.f;
        float riseLegTime = 0.2f; // Time for Golem to rise up its leg.
        float curRiseLegTime = 0.f;

        float attackRange = 20.f;
        float curAttackTime = 0.f;
        float attackTime = 2.f;

        float hurtEffectTime = 0.1f;
        float curHurtEffectTime = 0.f;
        Vector4 hurtEffectTint = Vector4(0.5f, 0.5f, 0.f, 1.f);

        float deathTime = 2.f;
        float curDeathTime = 0.f;

        void OnStart() override;

        void OnUpdate() override;

        void OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider) override;

    private:
        bool pause = false;

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

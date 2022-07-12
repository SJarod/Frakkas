#pragma once

#include "game/transform.hpp"
#include "ui/image_panel.hpp"
#include "ui/text.hpp"
#include "collider/sphere_collider.hpp"
#include "collider/box_collider.hpp"
#include "drawable/animated_draw.hpp"
#include "camera.hpp"
#include "sound.hpp"

#include "life.hpp"
#include "pause_trigger.hpp"
#include "player_mesh_rotation.hpp"

#include "component_generator.hpp"

namespace Game
{
    enum class EPlayerState
    {
        PAUSE,
        IDLE,
        WALK,
        DASH,
        ATTACK,
        DEATH
    };

    KK_COMPONENT(Player);

    public:
        SphereCollider* rigidBody = nullptr;
        BoxCollider* triggerCollider = nullptr;
        Life* life = nullptr;
        Sound* damageSound = nullptr;

        EPlayerState state = EPlayerState::IDLE;

        int damage = 3;

        Vector2 XZVelocity;
        Vector3 additionalTranslation; // Other movement component should use this to update translation

        void Scoring(const std::string_view& i_enemyName, int i_enemyScore);

        void GoToPause();
        void GoToIdle();
        void GoToWalk();
        void GoToDash();
        void GoToAttack();
        void GoToSuffer();
        void GoToDeath();


    private:
        static constexpr char triggerColliderEntityName[] = "Collider";
        static constexpr char weaponColliderEntityName[] = "WeaponCollider";

        AnimatedDraw* animDraw = nullptr;
        ImagePanel* bloodImage = nullptr;
        Camera* camera = nullptr;
        PauseTrigger* pauseTrigger = nullptr;
        PlayerMeshRotation* playerRotation = nullptr;
        Text* comboStreakText = nullptr;
        Text* comboScoreText = nullptr;

        BoxCollider* weaponCollider = nullptr;
        BoxCollider* comboWeaponCollider = nullptr;

        // ATTACK
        int baseDamage = 3;
        float lastAttackTimer = 0.f;
        float attackStreakTime = 1.f;
        int attackStreak = 0;

        float attackAnimSpeed = 1.75f;
        float attack1PlayTime = 100.f;
        float attack2PlayTime = 100.f;

        float weaponTriggerTime = 1.f; // Time during which the weapon collider is triggered
        float curWeaponTriggerTime = 0.f;
        float comboAttackTime = 2.f; // Time during which player can't move after combo attack
        float curComboAttackTime = 0.f;

        // COMBO
        int score = 0;
        int increaseComboKillCount = 5;
        int comboScoreFactor = 1;
        int comboStreak = 0;
        float comboTimer = 0.f;
        float comboStreakTime = 5.f;

        //  MOVEMENT
        float speed = 800.f;
        float speedWhenAttack = 300.f;

        // DASH
        float dashTime = 0.5f;
        float curDashTime = 0.f;
        float recoveryTime = 1.f;
        float curRecoveryTime = 1.f;
        float dashSpeed = 2000.f;

        // DAMAGE
        float hurtEffectTime = 0.5f;
        float curHurtEffectTime = 10.f;
        float hurtEffectSpeed = 5.f;
        
        // SOUNDS
        Sound* walkSound = nullptr;
        Sound* attackSound1 = nullptr;
        Sound* attackSound2 = nullptr;
        Sound* attackMissSound = nullptr;
        Sound* deathSound = nullptr;
        Sound* dashSound = nullptr;

        Sound* itemSound = nullptr;

        Sound* BGM = nullptr;
        Sound* BGM2 = nullptr;
        Sound* BGM3 = nullptr;

        float timeAfterDeath = 3.f;

        void OnStart() override;
        void OnUpdate() override;
        void OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider) override;

        void Move();
        void ListenInputs();
        void UpdateCombo();

        void Idle();
        void Walk();
        void Dash();
        void Death();
        void Attack();

        void SetAttackAnimSpeed();
        void SetMoveAnimSpeed();

        void SetSounds(Entity* i_child);
        void StopSounds();

    KK_COMPONENT_END
}


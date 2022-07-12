#include "pause_trigger.hpp"
#include "game_over_trigger.hpp"

#include "player.hpp"
#include "player_weapon.hpp"
#include "enemy_mesh_rotation.hpp"

#include "skeleton.hpp"

KK_COMPONENT_IMPL_BEGIN(Skeleton)

    KK_FIELD_PUSH(Skeleton, attackRange, EDataType::FLOAT)
    KK_FIELD_PUSH(Skeleton, attackTime, EDataType::FLOAT)
    KK_FIELD_PUSH(Skeleton, chargingTime, EDataType::FLOAT)
    KK_FIELD_PUSH(Skeleton, hurtEffectTime, EDataType::FLOAT)
    KK_FIELD_PUSH(Skeleton, hurtEffectTint, EDataType::FLOAT)
    KK_FIELD_COUNT(4)
    KK_FIELD_RANGE(0.f, 1.f)
    KK_FIELD_PUSH(Skeleton, deathTime, EDataType::FLOAT)

KK_COMPONENT_IMPL_END

void Skeleton::OnStart()
{
    owner.get()->name = "Skeleton_" + owner.get()->GetStringID();
    stats = owner.get()->GetComponent<EnemyStats>();
    life = owner.get()->GetComponent<Life>();
    life->SetMaxLife(stats->UpdateBaseLife(life->life));
    rigidBody = owner.get()->GetComponent<SphereCollider>();
    enemyMeshRotation = owner.get()->GetComponent<EnemyMeshRotation>();


    int index = 0;
    for (Entity* child : owner.get()->childs)
    {
        if (index == 0)
            animDraw = child->GetComponent<AnimatedDraw>();
        else if (index == 1)
            triggerCollider = child->GetComponent<BoxCollider>();
        else if (index == 2)
            weaponCollider = child->GetComponent<BoxCollider>();
        else if (index == 3)
            SetSounds(child);
        index++;
    }
    weaponCollider->enabled = false;
    animDraw->skmodel.sockets.front().transform.scale = Vector3(10000, 10000, 10000);

    Entity* pm = GetEntityContainer().FindEntityWithComponent<Player>();
    if (pm)
        playerTransform = &pm->transform;

    GoToRun();
 }

void Skeleton::OnUpdate()
{
    if (Inputs::IsPressed("pause"))
        pause = true;

    if (Time::GetTimeScale() == 0.f)
        StopSounds();

    switch(stats->state)
    {
        case EnemyState::RUNNING: Run(); break;
        case EnemyState::THINKING: Think(); break;
        case EnemyState::CHARGING: Charge(); break;
        case EnemyState::ATTACKING: Attack(); break;
        case EnemyState::SUFFERING: Suffer(); break;
        case EnemyState::DYING: Death(); break;
        default: break;
    }
}
void Skeleton::OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider)
{
    if (i_ownerCollider.owner.get()->name == "Collider")
    {
        if (auto weapon = i_otherCollider.owner.get()->GetComponent<PlayerWeapon>())
        {
            life->Hurt(weapon->damage);

            GoToSuffer();
        }
    }
}

void Skeleton::GoToRun()
{
    StopSounds();
    walkSound->loop = true;
    walkSound->Play();

    stats->state = EnemyState::RUNNING;

    animDraw->animGraph.PlayAnimation("Skeleton_Walk.fbx_mixamo.com", true);
}
void Skeleton::GoToThink()
{
    StopSounds();

    stats->state = EnemyState::THINKING;
    weaponCollider->enabled = false;
    enemyMeshRotation->enabled = true;

    rigidBody->velocity = Vector3(0.f, rigidBody->velocity.y, 0.f);

    animDraw->animGraph.PlayAnimation("Skeleton_Idle.fbx_mixamo.com", true);
}
void Skeleton::GoToCharge()
{
    stats->state = EnemyState::CHARGING;
    curChargingTime = 0.f;

    enemyMeshRotation->enabled = false;

    animDraw->animGraph.PlayAnimation("Skeleton_Slash.fbx_mixamo.com");
}
void Skeleton::GoToAttack()
{
    StopSounds();
    attackSound->Play();

    curAttackTime = 0.f;
    curThinkingTime = 0.f;
    stats->state = EnemyState::ATTACKING;

    weaponCollider->enabled = true;

    Position() = Position().get() + enemyMeshRotation->fromTo;

}
void Skeleton::GoToSuffer()
{
    StopSounds();
    hitSound->Play();

    stats->state = EnemyState::SUFFERING;
    enemyMeshRotation->enabled = false;
    rigidBody->velocity = Vector3::zero;

    curHurtEffectTime = 0.f;
    animDraw->skmodel.material.tint = hurtEffectTint;

    Inputs::RumbleGamepad(hurtEffectTime, 0.2f);
    animDraw->animGraph.PlayAnimation("Skeleton_Impact.fbx_mixamo.com");
}
void Skeleton::GoToDeath()
{
    StopSounds();
    deathSound->Play();

    triggerCollider->enabled = false;
    enemyMeshRotation->enabled = false;
    weaponCollider->enabled = false;
    rigidBody->velocity = Vector3::zero;
    stats->SpawnLifeItem();

    stats->state = EnemyState::DYING;

    // Notify player score
    GetEntityContainer().FindEntityWithComponent<Player>()->GetComponent<Player>()->Scoring(owner.get()->name, stats->score);

    Inputs::RumbleGamepad(hurtEffectTime, 0.4f);
    animDraw->animGraph.PlayAnimation("Skeleton_Death.fbx_mixamo.com");
}

void Skeleton::Run()
{
    walkSound->SetSoundPosition(Position().get());
    if (pause && Time::GetTimeScale() > 0.f) // Re-play sound if it had been stop by pause
    {
        walkSound->Play();
        pause = false;
    }

    vecDir = Vector3::VecFromPt(Position().get(), playerTransform->position.get()).Normalize() * stats->speed * Time::GetFixedDeltaTime();
    translation = Vector3(vecDir.x, rigidBody->velocity.y, vecDir.z);
    rigidBody->velocity = translation;

    distFromPlayer = Vector3::Distance(playerTransform->position.get(), Position().get());
    if (distFromPlayer < attackRange)
        GoToThink();

    if (!life->IsAlive())
        GoToDeath();
}
void Skeleton::Think()
{
    curThinkingTime += Time::GetDeltaTime() * stats->attackSpeed;
    distFromPlayer = Vector3::Distance(playerTransform->position.get(), Position().get());

    if (curThinkingTime > 1.f)
        GoToCharge();
    else if(distFromPlayer > attackRange + 5.f)
        GoToRun();

    if (!life->IsAlive())
        GoToDeath();
}
void Skeleton::Charge()
{
    curChargingTime += Time::GetDeltaTime();

    if (curChargingTime > chargingTime)
        GoToAttack();

    if (!life->IsAlive())
        GoToDeath();
}
void Skeleton::Attack()
{
    curAttackTime += Time::GetDeltaTime();

    if (curAttackTime >= attackTime)
        GoToThink();

    if (!life->IsAlive())
        GoToDeath();

    if (animDraw->animGraph.player.IsWaiting())
        animDraw->animGraph.PlayAnimation("Skeleton_Idle.fbx_mixamo.com", true);
}
void Skeleton::Suffer()
{
    curHurtEffectTime += Time::GetDeltaTime();
    if (curHurtEffectTime >= hurtEffectTime)
        animDraw->skmodel.material.tint = {0.f, 0.f, 0.f, 1.f};

    if (!life->IsInvincible())
        GoToThink();

    if (!life->IsAlive())
        GoToDeath();

    if (animDraw->animGraph.player.IsWaiting())
        animDraw->animGraph.PlayAnimation("Skeleton_Idle.fbx_mixamo.com", true);
}
void Skeleton::Death()
{
    curHurtEffectTime += Time::GetDeltaTime();
    if (curHurtEffectTime >= hurtEffectTime)
        animDraw->skmodel.material.tint = {0.f, 0.f, 0.f, 1.f};

    curDeathTime += Time::GetDeltaTime();
    if (curDeathTime > deathTime)
        owner.get()->destroy = true;
}

void Skeleton::StopSounds()
{
    walkSound->Pause();
    attackSound->Stop();
    hitSound->Stop();
}

void Skeleton::SetSounds(Entity* i_soundChild)
{
    attackSound = i_soundChild->GetComponents<Sound>()[0];
    walkSound = i_soundChild->GetComponents<Sound>()[1];
    hitSound = i_soundChild->GetComponents<Sound>()[2];
    deathSound = i_soundChild->GetComponents<Sound>()[3];
}
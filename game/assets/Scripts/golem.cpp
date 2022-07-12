#include "pause_trigger.hpp"
#include "game_over_trigger.hpp"

#include "player.hpp"
#include "player_weapon.hpp"

#include "golem.hpp"

KK_COMPONENT_IMPL_BEGIN(Golem)

    KK_FIELD_PUSH(Golem, attackRange, EDataType::FLOAT)
    KK_FIELD_PUSH(Golem, chargeTime, EDataType::FLOAT)
    KK_FIELD_PUSH(Golem, attackTime, EDataType::FLOAT)
    KK_FIELD_PUSH(Golem, riseLegTime, EDataType::FLOAT)
    KK_FIELD_PUSH(Golem, hurtEffectTime, EDataType::FLOAT)
    KK_FIELD_PUSH(Golem, hurtEffectTint, EDataType::FLOAT)
    KK_FIELD_COUNT(4)
    KK_FIELD_RANGE(0.f, 1.f)
    KK_FIELD_PUSH(Golem, deathTime, EDataType::FLOAT)

KK_COMPONENT_IMPL_END

void Golem::OnStart()
{
    owner.get()->name = "Golem_" + owner.get()->GetStringID();
    stats = owner.get()->GetComponent<EnemyStats>();
    life = owner.get()->GetComponent<Life>();
    life->SetMaxLife(stats->UpdateBaseLife(life->life));
    rigidBody = owner.get()->GetComponent<SphereCollider>();


    int index = 0;
    for (Entity* child : owner.get()->childs)
    {
        if (index == 0)
            animDraw = child->GetComponent<AnimatedDraw>();
        else if (index == 1)
            triggerCollider = child->GetComponent<BoxCollider>();
        else if (index == 2)
            attackDraw = child->GetComponent<StaticDraw>();
        else if (index == 3)
            SetSounds(child);
        index++;
    }
    attackDraw->enabled = true;
    attackDraw->Scale() = Vector3(attackRange, attackRange, 1.f);

    enemyMeshRotation = owner.get()->GetComponent<EnemyMeshRotation>();

    player = GetEntityContainer().FindEntityWithComponent<Player>()->GetComponent<Player>();
    Entity* playerMovement = GetEntityContainer().FindEntityWithComponent<Player>();
    if (playerMovement)
        playerTransform = &playerMovement->transform;

    GoToThink();
}

void Golem::OnUpdate()
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

void Golem::OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider)
{
    if (i_ownerCollider.owner.get()->name == "Collider")
    {
        if (auto weapon = i_otherCollider.owner.get()->GetComponent<PlayerWeapon>())
        {
            hitSound->Play();

            life->Hurt(weapon->damage);
            GoToSuffer();
        }
    }
}

void Golem::GoToRun()
{
    StopSounds();

    walkSound->Play();

    stats->state = EnemyState::RUNNING;

    animDraw->animGraph.PlayAnimation("Golem_Walk.fbx_mixamo.com", true);
}
void Golem::GoToThink()
{
    StopSounds();

    enemyMeshRotation->enabled = true;
    attackDraw->enabled = false;
    rigidBody->velocity = Vector3(0.f, rigidBody->velocity.y, 0.f);

    stats->state = EnemyState::THINKING;

    animDraw->skmodel.material.tint = {0.f, 0.f, 0.f, 1.f};
    animDraw->animGraph.player.playSpeed = 1.f;
    if (animDraw->animGraph.player.IsLooping())
        animDraw->animGraph.PlayAnimation("Golem_Idle.fbx_mixamo.com", true);
}
void Golem::GoToCharge()
{
    StopSounds();
    roarSound->Play();

    curChargeTime = 0.f;
    curThinkingTime = 0.f;
    curRiseLegTime = 0.f;
    enemyMeshRotation->enabled = false;

    stats->state = EnemyState::CHARGING;
    animDraw->animGraph.PlayAnimation("Golem_Stomp.fbx_mixamo.com");
}
void Golem::GoToAttack()
{
    StopSounds();
    attackSound->Play();

    animDraw->animGraph.player.playSpeed = 1.f;
    stats->state = EnemyState::ATTACKING;
    curAttackTime = 0.f;
}
void Golem::GoToSuffer()
{
    StopSounds();
    hitSound->Play();

    stats->state = EnemyState::SUFFERING;
    enemyMeshRotation->enabled = false;
    rigidBody->velocity = Vector3::zero;

    curHurtEffectTime = 0.f;
    animDraw->skmodel.material.tint = hurtEffectTint;

    Inputs::RumbleGamepad(hurtEffectTime, 0.4f);
    animDraw->animGraph.player.playSpeed = 1.5f;
    animDraw->animGraph.PlayAnimation("Golem_Hit.fbx_mixamo.com");
}
void Golem::GoToDeath()
{
    StopSounds();
    deathSound->Play();

    triggerCollider->enabled = false;
    enemyMeshRotation->enabled = false;
    attackDraw->enabled = false;

    rigidBody->velocity = Vector3::zero;
    stats->SpawnLifeItem();

    stats->state = EnemyState::DYING;

    // Notify player score
    GetEntityContainer().FindEntityWithComponent<Player>()->GetComponent<Player>()->Scoring(owner.get()->name, stats->score);

    Inputs::RumbleGamepad(hurtEffectTime, 0.6f);
    animDraw->animGraph.PlayAnimation("Golem_Death.fbx_mixamo.com");
}

void Golem::Run()
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
    if (distFromPlayer <= attackRange)
        GoToThink();

    if (!life->IsAlive())
        GoToDeath();
}
void Golem::Think()
{
    distFromPlayer = Vector3::Distance(playerTransform->position.get(), Position().get());
    rigidBody->velocity = Maths::Lerp(rigidBody->velocity, Vector3(0.f, rigidBody->velocity.y, 0.f), 0.05f);
    curThinkingTime += Time::GetDeltaTime() * stats->attackSpeed;

    if (animDraw->animGraph.player.IsWaiting())
        animDraw->animGraph.PlayAnimation("Golem_Idle.fbx_mixamo.com", true);

    if (curThinkingTime >= 1.f)
        GoToCharge();
    else if (distFromPlayer > attackRange)
        GoToRun();

    if (!life->IsAlive())
        GoToDeath();
}
void Golem::Charge()
{
    rigidBody->velocity = Vector3(0.f, rigidBody->velocity.y, 0.f);

    curChargeTime += Time::GetDeltaTime();
    curRiseLegTime += Time::GetDeltaTime();

    if (curRiseLegTime >= riseLegTime)
    {
        animDraw->animGraph.player.playSpeed = 0.f;
        attackDraw->enabled = true;
        attackDraw->model.material.tint = Vector4(0.f, 0.f, 0.f, (Maths::Abs(Maths::Cos(Time::GetTime() * 3.f)) + 0.5f) / 1.5f);
    }

    if (curChargeTime >= chargeTime)
        GoToAttack();

    if (!life->IsAlive())
        GoToDeath();
}
void Golem::Attack()
{
    distFromPlayer = Vector3::Distance(playerTransform->position.get(), Position().get());
    curAttackTime += Time::GetDeltaTime();

    if (curAttackTime >= attackTime)
    {
        if (distFromPlayer < attackRange)
        {
            player->life->Hurt(stats->damage);
            player->GoToSuffer();
        }

        GoToThink();
    }

    if (!life->IsAlive())
        GoToDeath();

    if (animDraw->animGraph.player.IsWaiting())
        animDraw->animGraph.PlayAnimation("Golem_Idle.fbx_mixamo.com", true);
}
void Golem::Suffer()
{
    curHurtEffectTime += Time::GetDeltaTime();
    if (curHurtEffectTime >= hurtEffectTime)
        animDraw->skmodel.material.tint = {0.f, 0.f, 0.f, 1.f};

    if (!life->IsInvincible())
        GoToThink();

    if (!life->IsAlive())
        GoToDeath();

    if (animDraw->animGraph.player.IsWaiting())
        animDraw->animGraph.PlayAnimation("Golem_Idle.fbx_mixamo.com", true);
}
void Golem::Death()
{
    curHurtEffectTime += Time::GetDeltaTime();
    if (curHurtEffectTime >= hurtEffectTime)
        animDraw->skmodel.material.tint = {0.f, 0.f, 0.f, 1.f};
    if (pause && Time::GetTimeScale() > 0.f)
    {
        pause = false;
        deathSound->Play();
    }

    if (curDeathTime > deathTime)
        owner.get()->destroy = true;

    curDeathTime += Time::GetDeltaTime();
}

void Golem::StopSounds()
{
    walkSound->Pause();
    deathSound->Pause();
    hitSound->Stop();
}

void Golem::SetSounds(Entity* i_soundChild)
{
    attackSound = i_soundChild->GetComponents<Sound>()[0];
    roarSound = i_soundChild->GetComponents<Sound>()[1];
    walkSound = i_soundChild->GetComponents<Sound>()[2];
    hitSound = i_soundChild->GetComponents<Sound>()[3];
    deathSound = i_soundChild->GetComponents<Sound>()[4];
}

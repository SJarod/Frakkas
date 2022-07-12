#include "enemy_weapon.hpp"
#include "game_over_trigger.hpp"
#include "life_item.hpp"
#include "follow_camera.hpp"
#include "ui/button.hpp"
#include "world_data.hpp"

#include "player.hpp"

KK_COMPONENT_IMPL_BEGIN(Player)

    KK_FIELD_PUSH_TEXT("Movement")
    KK_FIELD_PUSH(Player, speed, EDataType::FLOAT)
    KK_FIELD_PUSH(Player, speedWhenAttack, EDataType::FLOAT)

    KK_FIELD_PUSH_TEXT("Dash")
    KK_FIELD_PUSH(Player, dashTime, EDataType::FLOAT)
    KK_FIELD_PUSH(Player, dashSpeed, EDataType::FLOAT)
    KK_FIELD_PUSH(Player, recoveryTime, EDataType::FLOAT)

    KK_FIELD_PUSH_TEXT("Attack")
    KK_FIELD_PUSH(Player, baseDamage, EDataType::INT)
    KK_FIELD_PUSH(Player, attackStreakTime, EDataType::FLOAT)
    KK_FIELD_PUSH(Player, weaponTriggerTime, EDataType::FLOAT)
    KK_FIELD_TOOLTIP("Time during which the weapon collider is triggered")
    KK_FIELD_PUSH(Player, comboAttackTime, EDataType::FLOAT)
    KK_FIELD_TOOLTIP("Time during which player can't move after combo attack")
    KK_FIELD_PUSH(Player, attackAnimSpeed, EDataType::FLOAT)
    KK_FIELD_PUSH(Player, attack1PlayTime, EDataType::FLOAT)
    KK_FIELD_PUSH(Player, attack2PlayTime, EDataType::FLOAT)

    KK_FIELD_PUSH_TEXT("Scoring")
    KK_FIELD_PUSH(Player, increaseComboKillCount, EDataType::INT)
    KK_FIELD_RENAME("combo kill count")
    KK_FIELD_TOOLTIP("The number of kill to process to increase score combo")
    KK_FIELD_PUSH(Player, comboStreakTime, EDataType::FLOAT)

    KK_FIELD_PUSH_TEXT("Hurt")
    KK_FIELD_PUSH(Player, hurtEffectTime, EDataType::FLOAT)
    KK_FIELD_PUSH(Player, hurtEffectSpeed, EDataType::FLOAT)
    KK_FIELD_PUSH(Player, timeAfterDeath, EDataType::FLOAT)
    KK_FIELD_TOOLTIP("Waiting time after player death before trigger Game over screen")

KK_COMPONENT_IMPL_END

void Player::OnStart()
{
    // INPUTS
    Inputs::SetButtonAction("attack", {EButton::SPACE, EButton::MOUSE_LEFT, EButton::GAMEPAD_A, EButton::GAMEPAD_X}, true);
    Inputs::SetButtonAction("pause", {EButton::ESCAPE, EButton::P, EButton::GAMEPAD_START});
    Inputs::SetButtonAction("dash", {EButton::LEFT_SHIFT, EButton::RIGHT_SHIFT,EButton::E, EButton::MOUSE_RIGHT, EButton::GAMEPAD_B, EButton::GAMEPAD_Y, EButton::GAMEPAD_SHOULDER_RIGHT}, true);

    owner.get()->name = "Player";

    rigidBody = owner.get()->GetComponent<SphereCollider>();

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
            comboWeaponCollider = child->GetComponent<BoxCollider>();
        else if (index == 4)
        {
            comboStreakText = child->GetComponents<Text>()[0];
            comboScoreText = child->GetComponents<Text>()[1];
        }
        else if (index  == 5)
            SetSounds(child);
        index++;
    }

    weaponCollider->enabled = false;
    comboWeaponCollider->enabled = false;

    life = owner.get()->GetComponent<Life>();
    bloodImage = owner.get()->GetComponent<ImagePanel>();
    bloodImage->enabled = true;
    camera = GetEntityContainer().FindEntityWithComponent<FollowCamera>()->GetComponent<Camera>();
    pauseTrigger = GetEntityContainer().FindEntityWithComponent<PauseTrigger>()->GetComponent<PauseTrigger>();
    pauseTrigger->owner.get()->GetComponents<Button>()[0]->AddClickEvent([&]()
    {
        GoToIdle();
    });
    playerRotation = owner.get()->GetComponent<PlayerMeshRotation>();

    // Set Player life according to world data
    auto worldData = World::GetWorldData<FrakkarenaWorldData>();
    if (worldData)
    {
        if (worldData->level == 0)
            worldData->playerLife = life->GetMaxLife();
        else
            life->Hurt(life->GetMaxLife() - worldData->playerLife);
    }

    BGM->Play();

    GoToIdle();
}

void Player::OnUpdate()
{
    Vector3 forward = Vector3::VecFromPt(camera->Position(), Position()).Normalize();
    Sound::SetMainListener(Position(), Vector3(forward.x, 0.f, forward.z));

    if (state != EPlayerState::DEATH && !life->IsAlive())
        GoToDeath();

    bloodImage->tint.w = 1.f - life->ratio;
    curRecoveryTime += Time::GetDeltaTime();

    float prevHurtTime = curHurtEffectTime;
    curHurtEffectTime += Time::GetDeltaTime();
    if (curHurtEffectTime < hurtEffectTime)
        animDraw->skmodel.material.tint = Vector4(Maths::Abs(Maths::Cos(Time::GetTime() * hurtEffectSpeed)), 0.f, 0.f, 1.f);
    else if (prevHurtTime < hurtEffectTime)
        animDraw->skmodel.material.tint = Vector4(0.f, 0.f, 0.f, 1.f);

    switch(state)
    {
        case EPlayerState::PAUSE:
            if (Inputs::IsPressed("pause"))
            {
                pauseTrigger->enabled = false;
                GoToIdle();
            }
            break;
        case EPlayerState::IDLE:
            Idle();
            break;
        case EPlayerState::WALK:
            Walk();
            break;
        case EPlayerState::DASH:
            Dash();
            break;
        case EPlayerState::ATTACK:
            Attack();
            break;
        case EPlayerState::DEATH:
            Death();
            break;
    }
}

void Player::OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider)
{
    if (i_ownerCollider.owner.get()->name == triggerColliderEntityName)
    {
        if(auto weapon = i_otherCollider.owner.get()->GetComponent<EnemyWeapon>())
        {
            life->Hurt(weapon->damage);
            GoToSuffer();
        }

        if (auto lifeItem = i_otherCollider.owner.get()->GetComponent<LifeItem>())
        {
            life->Heal(lifeItem->life);
            i_otherCollider.owner.get()->destroy = true;
            itemSound->Play();
            Inputs::RumbleGamepadPro(0.1f, 0.f, 0.1f);
        }
    }
}

void Player::Move()
{
    float forwardVelocity = Inputs::GetAxis("forward");
    float strafeVelocity = Inputs::GetAxis("horizontal");

    Vector2 gamepadDir = Inputs::GetLeftJoystickDirection();

    float frameSpeed = (state == EPlayerState::ATTACK ? speedWhenAttack : speed) * Time::GetFixedDeltaTime();

    strafeVelocity = Maths::Clamp( strafeVelocity + gamepadDir.x, -1.f, 1.f) * frameSpeed;
    forwardVelocity = Maths::Clamp( forwardVelocity + gamepadDir.y, -1.f, 1.f) * frameSpeed;

    Vector3 rot = camera->Rotation();
    XZVelocity.x = Maths::Sin(rot.y) * forwardVelocity + Maths::Cos(rot.y) * strafeVelocity;
    XZVelocity.y = Maths::Cos(rot.y) * forwardVelocity - Maths::Sin(rot.y) * strafeVelocity;

    Vector3 translation = Vector3(XZVelocity.x, rigidBody->velocity.y, -XZVelocity.y);
    rigidBody->velocity = translation + additionalTranslation;

    additionalTranslation = Vector3::zero;
}

void Player::ListenInputs()
{
    if (Inputs::IsPressed("pause"))
        GoToPause();
    else if (Inputs::IsPressed("attack"))
        GoToAttack();
    else if (curRecoveryTime > recoveryTime && Inputs::IsPressed("dash"))
        GoToDash();
}

void Player::UpdateCombo()
{
    lastAttackTimer += Time::GetDeltaTime();
    comboTimer += Time::GetDeltaTime();

    if (comboStreak >= increaseComboKillCount)
    {
        comboScoreFactor++;
        comboStreak = 0;
    }
    if (comboTimer >= comboStreakTime)
    {
        comboTimer = 0.f;
        comboScoreFactor = 1;
        comboStreak = 0;
    }
    if (lastAttackTimer >= attackStreakTime)
        attackStreak = 0;

    comboStreakText->text = StringFormat::GetFormat("x", comboScoreFactor-1);
    if (auto worldData = World::GetWorldData<FrakkarenaWorldData>())
        comboScoreText->text = StringFormat::GetFormat(worldData->levelComboScore);
}

void Player::Scoring(const std::string_view& i_enemyName, int i_enemyScore)
{
    if (auto worldData = World::GetWorldData<FrakkarenaWorldData>())
    {
        if (i_enemyName.find("Skeleton") != std::string_view::npos)
            worldData->skeletonCount++;
        else if (i_enemyName.find("Golem") != std::string_view::npos)
            worldData->golemCount++;

        worldData->levelComboScore += i_enemyScore * comboScoreFactor - i_enemyScore; // Count only combo points
    }

    comboTimer = 0.f;
    comboStreak++;
}

void Player::GoToPause()
{
    StopSounds();

    state = EPlayerState::PAUSE;
    pauseTrigger->enabled = true;
}
void Player::GoToIdle()
{
    StopSounds();

    SetMoveAnimSpeed();
    weaponCollider->enabled = false;
    comboWeaponCollider->enabled = false;
    playerRotation->enabled = true;
    if (animDraw->animGraph.player.IsLooping())
        animDraw->animGraph.PlayAnimation("Hero_Idle_1.fbx_mixamo.com", true);

    state = EPlayerState::IDLE;
}
void Player::GoToWalk()
{
    StopSounds();

    if (animDraw->animGraph.player.IsLooping())
        animDraw->animGraph.PlayAnimation("Hero_Walk.fbx_mixamo.com", true);

    state = EPlayerState::WALK;

    walkSound->Play();
}
void Player::GoToDash()
{
    StopSounds();

    state = EPlayerState::DASH;

    curRecoveryTime = 0.f;
    curDashTime = 0.f;

    Inputs::RumbleGamepadPro(0.05f, 0.f, 0.1f);
    animDraw->animGraph.PlayAnimation("Hero_Run.fbx_mixamo.com", true);

    dashSound->Play();
}
void Player::GoToAttack()
{
    StopSounds();

    state = EPlayerState::ATTACK;

    // COMBO
    attackStreak = Maths::Modulo(attackStreak++, 3)+1;

    // TIMER
    lastAttackTimer = 0.f;
    curComboAttackTime = 0.f;
    curWeaponTriggerTime = 0.f;

    // DAMAGE
    damage = baseDamage * attackStreak;

    // FORCE MOVEMENT TO AWAKE COLLIDER
    Vector2 normVelocity = playerRotation->direction;
    Position() = Position().get() + Vector3(normVelocity.x, 0.f, -normVelocity.y);

    // WEAPON TRIGGER
    playerRotation->enabled = false;

    if (attackStreak < 3)
        weaponCollider->enabled = true;
    else
        comboWeaponCollider->enabled = true;

    // ANIMATION
    SetAttackAnimSpeed();
    switch(attackStreak)
    {
        case 2:
            animDraw->animGraph.PlayAnimation("Hero_Slash_3.fbx_mixamo.com");
            animDraw->animGraph.player.playTime = attack2PlayTime;
            break;
        case 3:
            animDraw->animGraph.PlayAnimation("Hero_Slash_4.fbx_mixamo.com");
            break;
        case 1:
        default:
            animDraw->animGraph.PlayAnimation("Hero_Slash_1.fbx_mixamo.com");
            animDraw->animGraph.player.playTime = attack1PlayTime;
            break;
    }

    std::random_device randDevice;
    std::mt19937_64 gen(randDevice());
    std::uniform_int_distribution<> discreteDistri(0, 1);

    if (discreteDistri(gen) == 0)
        attackSound1->Play();
    else
        attackSound2->Play();

    attackMissSound->Play();
}
void Player::GoToDeath()
{
    StopSounds();

    state = EPlayerState::DEATH;
    animDraw->animGraph.PlayAnimation("Hero_Death.fbx_mixamo.com");
    Inputs::RumbleGamepad(timeAfterDeath/4.f, 0.5f);

    owner.get()->childs.back()->enabled = false;
    bloodImage->enabled = false;
    rigidBody->velocity = Vector3::zero;
    playerRotation->enabled = false;
    weaponCollider->enabled = false;
    comboWeaponCollider->enabled = false;
    triggerCollider->enabled = false;

    deathSound->Play();
}

void Player::Idle()
{
    Move();

    if (animDraw->animGraph.player.IsWaiting())
        animDraw->animGraph.PlayAnimation("Hero_Idle_1.fbx_mixamo.com", true);

    if (XZVelocity != Vector2())
        GoToWalk();

    UpdateCombo();
    ListenInputs();
}
void Player::Walk()
{
    Move();

    if (animDraw->animGraph.player.IsWaiting())
        animDraw->animGraph.PlayAnimation("Hero_Walk.fbx_mixamo.com", true);

    if (XZVelocity == Vector2())
        GoToIdle();

    UpdateCombo();
    ListenInputs();
}
void Player::Dash()
{
    curDashTime += Time::GetDeltaTime();

    Vector2 dashVelocity = XZVelocity * dashSpeed * Time::GetFixedDeltaTime();
    additionalTranslation = additionalTranslation + Vector3(dashVelocity.x, 0.f, -dashVelocity.y);

    float prevLength = XZVelocity.Length();

    Move();

    if (prevLength == 0.f && XZVelocity.Length() > 0.f)
        animDraw->animGraph.PlayAnimation("Hero_Run.fbx_mixamo.com", true);
    else if (prevLength > 0.f && XZVelocity.Length() == 0.f)
        animDraw->animGraph.PlayAnimation("Hero_Idle_1.fbx_mixamo.com", true);

    if (curDashTime > dashTime || Inputs::IsReleased("dash"))
        GoToIdle();

    UpdateCombo();
    ListenInputs();
}
void Player::Attack()
{
    if (animDraw->animGraph.player.IsWaiting())
        animDraw->animGraph.PlayAnimation("Hero_Idle_1.fbx_mixamo.com", true);

    curComboAttackTime += Time::GetDeltaTime();
    curWeaponTriggerTime += Time::GetDeltaTime();

    if (attackStreak < 3)
        Move();
    else
        rigidBody->velocity = Vector3::zero;

    if (curWeaponTriggerTime > weaponTriggerTime)
    {
        if (attackStreak < 3)
            GoToIdle();
    }
    else if (curWeaponTriggerTime > weaponTriggerTime)
        GoToIdle();

    UpdateCombo();
}
void Player::GoToSuffer()
{
    curHurtEffectTime = 0.f;
    animDraw->skmodel.material.tint = {1.f, 0.f, 0.f, 1.f};
    damageSound->Play();
    Inputs::RumbleGamepadPro(hurtEffectTime, 0.4f, 0.2f);
}
void Player::Death()
{
    timeAfterDeath -= Time::GetDeltaTime();

    if (timeAfterDeath <= 0.f)
        GetEntityContainer().FindEntityWithComponent<GameOverTrigger>()->GetComponent<GameOverTrigger>()->enabled = true;
}

void Player::SetAttackAnimSpeed()
{
    animDraw->animGraph.player.playSpeed = attackAnimSpeed;
}

void Player::SetMoveAnimSpeed()
{
    animDraw->animGraph.player.playSpeed = 1.f;
}

void Player::SetSounds(Entity* i_child)
{
    walkSound = i_child->GetComponents<Sound>()[0];
    attackSound1 = i_child->GetComponents<Sound>()[1];
    attackSound2 = i_child->GetComponents<Sound>()[2];
    attackMissSound = i_child->GetComponents<Sound>()[3];
    damageSound = i_child->GetComponents<Sound>()[4];
    deathSound = i_child->GetComponents<Sound>()[5];
    dashSound = i_child->GetComponents<Sound>()[6];
    itemSound = i_child->GetComponents<Sound>()[7];
    BGM = i_child->GetComponents<Sound>()[8];
}

void Player::StopSounds()
{
    walkSound->Stop();
    attackSound1->Stop();
    attackSound2->Stop();
    attackMissSound->Stop();
    damageSound->Stop();
    deathSound->Stop();
    dashSound->Stop();
    itemSound->Stop();
}

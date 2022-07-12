#include "life.hpp"

KK_COMPONENT_IMPL_BEGIN(Life)

    KK_FIELD_PUSH(Life, maxLife, EDataType::INT)
    KK_FIELD_RENAME("life")
    KK_FIELD_PUSH(Life, invincibilityTime, EDataType::FLOAT)

KK_COMPONENT_IMPL_END

void Life::OnStart()
{
    life = maxLife;
    curInvincibilityTime = invincibilityTime;
}

void Life::OnUpdate()
{
    if (IsInvincible())
        curInvincibilityTime += Time::GetDeltaTime();
}

void Life::Heal(int i_heal)
{
    life = Maths::Clamp(life + i_heal, -1, maxLife);
    ratio = static_cast<float>(life) / static_cast<float>(maxLife);
}

void Life::Hurt(int i_damage)
{
    if (!IsInvincible())
    {
        life = Maths::Clamp(life - i_damage, -1, maxLife);
        curInvincibilityTime = 0.f;
        ratio = static_cast<float>(life) / static_cast<float>(maxLife);
    }
}

bool Life::IsAlive() const
{
    return life > 0;
}

bool Life::IsInvincible() const
{
    return curInvincibilityTime < invincibilityTime;
}

int Life::GetMaxLife() const
{
    return maxLife;
}

void Life::SetMaxLife(int i_maxLife, bool i_resetLife)
{
    maxLife = i_maxLife;
    life = i_resetLife ? i_maxLife : life;
}


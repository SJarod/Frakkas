#pragma once

#include "component_generator.hpp"

namespace Game
{
    KK_COMPONENT(Life);

        int life = 30;
        float ratio = 1.f;

        float invincibilityTime = 1.f;

        void OnStart() override;

        void OnUpdate() override;

        void Heal(int i_heal);
        void Hurt(int i_damage);

        bool IsAlive() const;
        bool IsInvincible() const;

        int GetMaxLife() const;
        void SetMaxLife(int i_maxLife, bool i_resetLife = true);

    private:
        int maxLife = 30;

        float curInvincibilityTime = 0.f;

    KK_COMPONENT_END
}

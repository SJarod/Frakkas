#pragma once

#include "component_generator.hpp"

namespace Game
{

    enum class EnemyState
    {
        RUNNING,
        THINKING,
        CHARGING,
        ATTACKING,
        SUFFERING,
        DYING
    };

    KK_COMPONENT(EnemyStats);

        std::string_view name;

        int life;
        float speed;
        int damage;
        float attackSpeed;
        int score;
        EnemyState state = EnemyState::RUNNING;

        /// You can remove the functions you don't need.
        void OnStart() override;

        void OnUpdate() override;

        void ApplyWorldLevel();


        // Function used by enemies when they die
        void SpawnLifeItem();

        int UpdateBaseLife(int i_newBaseLife);

        int GetBaseScore() const {return baseScore;};

    private:
        float increaseRatio = 0.5f;
        float itemDropRate = 1.f / 4.f;

        int baseLife = 3;
        float baseSpeed = 900;
        int baseDamage = 3;
        float baseAttackSpeed = 0.5f;
        int baseScore = 5;

    KK_COMPONENT_END
}

#include <random>

#include "world_data.hpp"
#include "player.hpp"
#include "life_item.hpp"

#include "enemy_stats.hpp"

KK_COMPONENT_IMPL_BEGIN(EnemyStats)

    KK_FIELD_PUSH(EnemyStats, baseLife, EDataType::INT)
    KK_FIELD_PUSH(EnemyStats, baseDamage, EDataType::INT)
    KK_FIELD_PUSH(EnemyStats, baseSpeed, EDataType::FLOAT)
    KK_FIELD_PUSH(EnemyStats, baseAttackSpeed, EDataType::FLOAT)
    KK_FIELD_TOOLTIP("Attack per seconds")
    KK_FIELD_PUSH(EnemyStats, baseScore, EDataType::INT)
    KK_FIELD_PUSH_TEXT("")
    KK_FIELD_PUSH(EnemyStats, increaseRatio, EDataType::FLOAT)
    KK_FIELD_TOOLTIP("The increase percentage when stat level up")
    KK_FIELD_RANGE(0.f, 10.f)

KK_COMPONENT_IMPL_END

void EnemyStats::OnStart()
{
    ApplyWorldLevel();
}

void EnemyStats::OnUpdate()
{}

void EnemyStats::ApplyWorldLevel()
{
    if (auto worldData = World::GetWorldData<FrakkarenaWorldData>())
    {
        life = baseLife + baseLife * (worldData->lifeLevel * increaseRatio);
        damage = baseDamage + baseDamage * (worldData->damageLevel * increaseRatio);
        attackSpeed = baseAttackSpeed + baseAttackSpeed * (worldData->damageSpeedLevel * increaseRatio);
        speed = baseSpeed + baseSpeed * (worldData->speedLevel * increaseRatio);
        score = baseScore;
    }
}

void EnemyStats::SpawnLifeItem()
{
    float playerLifeRatio = GetEntityContainer().FindEntityWithComponent<Player>()->GetComponent<Life>()->ratio;
    float spawnItemProb = (1.f - playerLifeRatio) * itemDropRate;
    std::random_device randDevice;
    std::mt19937_64 gen(randDevice());
    std::uniform_real_distribution<> discreteDistri(0.f, 1.f);

    if (discreteDistri(gen) <= spawnItemProb)
    {
        Entity* item = GetEntityContainer().CreateEntity("LifeItem");
        item->AddComponent<LifeItem>();
        item->transform.position = Position().get() + Vector3::up * 5.f;
        item->transform.rotation = Vector3(0.f, -45.f, 0.f);
        item->transform.scale = Vector3(1.f, 1.f, 2.f);
    }
}

int EnemyStats::UpdateBaseLife(int i_newBaseLife)
{
    baseLife = i_newBaseLife;
    if (auto worldData = World::GetWorldData<FrakkarenaWorldData>())
        life = baseLife + baseLife * (worldData->lifeLevel * increaseRatio);

    return life;
}
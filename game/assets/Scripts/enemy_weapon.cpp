#include "enemy_stats.hpp"

#include "enemy_weapon.hpp"

KK_COMPONENT_IMPL_BEGIN(EnemyWeapon)

KK_COMPONENT_IMPL_END

void EnemyWeapon::OnStart()
{
    auto stats = owner.get()->GetRootEntity()->GetComponent<EnemyStats>();
    stats->ApplyWorldLevel();
    damage = stats->damage;
}


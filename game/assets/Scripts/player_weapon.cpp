#include "player.hpp"
#include "enemy_stats.hpp"

#include "player_weapon.hpp"

KK_COMPONENT_IMPL_BEGIN(PlayerWeapon)

KK_COMPONENT_IMPL_END

void PlayerWeapon::OnStart()
{
    player = owner.get()->GetRootEntity()->GetComponent<Player>();
}

void PlayerWeapon::OnUpdate()
{
    damage = player->damage;
}

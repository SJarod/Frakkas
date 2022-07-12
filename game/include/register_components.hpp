#pragma once

//Add include $i
#include "physic_goal_trigger.hpp"
#include "physic_ball_object.hpp"
#include "physic_ball_trigger.hpp"
#include "physic_player.hpp"
#include "level_selector.hpp"
#include "portal_manager.hpp"
#include "life_item.hpp"
#include "pause_trigger.hpp"
#include "life.hpp"
#include "game_over_trigger.hpp"
#include "player_weapon.hpp"
#include "enemy_weapon.hpp"
#include "enemy_stats.hpp"
#include "enemy_mesh_rotation.hpp"
#include "menu_script.hpp"
#include "player_mesh_rotation.hpp"
#include "fps_movement.hpp"
#include "follow_camera.hpp"
#include "player.hpp"
#include "golem.hpp"
#include "skeleton.hpp"
#include "spawner.hpp"
#include "portal.hpp"
#include "ui/panel.hpp"
#include "ui/image_panel.hpp"
#include "ui/image_button.hpp"
#include "ui/text.hpp"
#include "ui/button.hpp"
#include "drawable/static_draw.hpp"
#include "drawable/animated_draw.hpp"
#include "camera.hpp"
#include "sound.hpp"
#include "game/lowcomponent/drawable.hpp"

#include "game/entity.hpp"
#include "game/entity_manager.hpp"

namespace Game
{
    void RegisterComponents(EntityManager& io_entityManager)
    {
        Entity greedEntity(0, "greed");

        // Add new component $c
		greedEntity.AddComponent<PhysicGoalTrigger>();
		greedEntity.AddComponent<PhysicBallObject>();
		greedEntity.AddComponent<PhysicBallTrigger>();
		greedEntity.AddComponent<PhysicPlayer>();
		greedEntity.AddComponent<LevelSelector>();
		greedEntity.AddComponent<PortalManager>();
		greedEntity.AddComponent<LifeItem>();
		greedEntity.AddComponent<PauseTrigger>();
		greedEntity.AddComponent<Life>();
		greedEntity.AddComponent<GameOverTrigger>();
		greedEntity.AddComponent<PlayerWeapon>();
		greedEntity.AddComponent<EnemyWeapon>();
		greedEntity.AddComponent<EnemyStats>();
		greedEntity.AddComponent<EnemyMeshRotation>();
        greedEntity.AddComponent<Player>();
        greedEntity.AddComponent<Spawner>();
        greedEntity.AddComponent<Portal>();
        greedEntity.AddComponent<Golem>();
        greedEntity.AddComponent<Skeleton>();
        greedEntity.AddComponent<Panel>();
        greedEntity.AddComponent<Button>();
        greedEntity.AddComponent<ImageButton>();
        greedEntity.AddComponent<Text>();
        greedEntity.AddComponent<ImagePanel>();
		greedEntity.AddComponent<MenuScript>();
        greedEntity.AddComponent<PlayerMeshRotation>();
        greedEntity.AddComponent<StaticDraw>();
        greedEntity.AddComponent<AnimatedDraw>();
        greedEntity.AddComponent<Camera>();
        greedEntity.AddComponent<Sound>();
        greedEntity.AddComponent<FPSMovement>();
        greedEntity.AddComponent<FollowCamera>();
        greedEntity.AddComponent<Drawable>();
    }
}

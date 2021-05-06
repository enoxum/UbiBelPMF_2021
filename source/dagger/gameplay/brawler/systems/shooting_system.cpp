#include "shooting_system.h"

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include "gameplay/brawler/components/bullet.h"
#include "gameplay/brawler/components/movable.h"
#include "gameplay/brawler/components/player.h"
#include "gameplay/brawler/entities/bullet.h"
#include "gameplay/brawler/weapon.h"

using namespace brawler;
using namespace dagger;

void ShootingSystem::Run()
{

	auto view = Engine::Registry().view<Sprite, InputReceiver, Player, Transform, Movable>();

    view.each([](auto &sprite, auto &input, auto& player, auto& transform, auto& movable) {
        // TODO cooldown
        if (player.active_weapon_idx == -1)
            return;
        auto& weapon = player.weapons[player.active_weapon_idx];
        if (EPSILON_NOT_ZERO(input.Get("attack")))
        {
            if (!weapon.shoot())
                return;

            movable.speed.x -= sprite.scale.x * weapon.recoil();
	        BulletEntity::Create(weapon.bulletSize(), weapon.damage(), transform.position, sprite.scale.x>=0.0f? 1 : -1);
            return;
        }

        if (EPSILON_NOT_ZERO(input.Get("reload")))
        {   
            weapon.reload();
            return;
        }

        if (EPSILON_NOT_ZERO(input.Get("cycle_weapons"))) 
        {
            auto num_weapons = player.weapons.size();
            if (num_weapons < 2)
                return;

            player.active_weapon_idx = (player.active_weapon_idx + 1) % num_weapons;

            return;
        }

    });
}

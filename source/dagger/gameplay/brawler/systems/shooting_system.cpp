#include "shooting_system.h"
#include "iostream"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include "gameplay/brawler/components/bullet.h"
#include "gameplay/brawler/components/movable.h"
#include "gameplay/brawler/components/player.h"
#include "gameplay/brawler/entities/bullet.h"
#include "gameplay/brawler/entities/projectile.h"
#include "gameplay/brawler/weapon.h"

using namespace brawler;
using namespace dagger;

Bool ShootingSystem::isProjectile(WeaponType wp)
{
    switch (wp)
    {
    case WeaponType::C4:
        return true;
        break;
    case WeaponType::GRANADE:
        return true;
        break;
    case WeaponType::FLASH:
        return true;
        break;
    case WeaponType::MINE:
        return true;
        break;
    case WeaponType::MEDKIT:
        return true;
        break;
    case WeaponType::BANANA:
        return true;
        break;
    default:
        return false;
        break;
    }
}

void ShootingSystem::Run()
{

	auto view = Engine::Registry().view<Sprite, InputReceiver, Player, Transform, Movable>();

    view.each([](auto &sprite, auto &input, auto& player, auto& transform, auto& movable) {
        // TODO cooldown

        auto& t = Engine::Registry().get<Transform>(player.currentWeapon);
        auto& s = Engine::Registry().get<Sprite>(player.currentWeapon);

        s.scale.x = sprite.scale.x;

        t.position.x = transform.position.x;
        t.position.y = transform.position.y;
        t.position.z = 0.0f;
        
        if (player.active_weapon_idx == -1)
            return;
        auto& weapon = player.weapons[player.active_weapon_idx];
        if (EPSILON_NOT_ZERO(input.Get("attack")))
        {
            if (!weapon.shoot())
                return;

            movable.speed.x -= sprite.scale.x * weapon.recoil();

            if(!isProjectile(weapon.weaponType())){
                BulletEntity::Create(weapon.weaponType(), weapon.bulletSize(), weapon.damage(), transform.position, sprite.scale.x>=0.0f? 1 : -1);
            }else{
                ProjectileEntity::Create(weapon.weaponType(), weapon.bulletSize(), weapon.damage(), transform.position, sprite.scale.x>=0.0f? 1 : -1);
            }
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
            Weapon wp = player.weapons[player.active_weapon_idx];
            auto& weaponSprite = Engine::Registry().get<Sprite>(player.currentWeapon);
            weaponSprite.color = {1.0f, 1.0f, 1.0f, 1.0f};
            AssignSprite(weaponSprite, "brawler:" + wp.sprite());
            return;
        }

    });
}

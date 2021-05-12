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

Bool ShootingSystem::isConsumable(WeaponType wp)
{
    switch (wp)
    {
    case WeaponType::BANANA:
        return true;
        break;
    case WeaponType::MEDKIT:
        return true;
        break;
    default:
        return false;
        break;
    }
}

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
    default:
        return false;
        break;
    }
}

void ShootingSystem::editSprite(Entity& currWp, Weapon& wp, double alpha){
    auto& weaponSprite = Engine::Registry().get<Sprite>(currWp);
    weaponSprite.color = {1.0f, 1.0f, 1.0f, alpha};
    weaponSprite.scale = wp.scale();
    AssignSprite(weaponSprite, "brawler:" + wp.sprite());
}

void ShootingSystem::Run()
{

	auto view = Engine::Registry().view<Sprite, InputReceiver, Player, Transform, Movable>();

    view.each([](auto &sprite, auto &input, auto& player, auto& transform, auto& movable) {
        // TODO cooldown

        auto& t = Engine::Registry().get<Transform>(player.currentWeapon);
        auto& s = Engine::Registry().get<Sprite>(player.currentWeapon);

        if (player.active_weapon_idx == -1)
            return;

        auto& weapon = player.weapons[player.active_weapon_idx];

        s.scale.x = sprite.scale.x;
        int dir = sprite.scale.x>=0.0f? 1 : -1;

        int trans_x = weapon.translate().x;
        int trans_y = weapon.translate().y;

        t.position.x = transform.position.x + trans_x * dir;
        t.position.y = transform.position.y + trans_y;
        t.position.z = 0.0f;
        
        if (EPSILON_NOT_ZERO(input.Get("attack")))
        {
            if (!weapon.shoot()){
                return;
            }

            if(isProjectile(weapon.weaponType())){
                movable.speed.x -= sprite.scale.x * weapon.recoil();
                Vector3 pos = {transform.position.x + trans_x, transform.position.y + trans_y, transform.position.z};
                ProjectileEntity::Create(weapon.weaponType(), weapon.bulletSize(), weapon.damage(), pos, sprite.scale.x>=0.0f? 1 : -1);
            }else if(isConsumable(weapon.weaponType())){
        
            }else{
                movable.speed.x -= sprite.scale.x * weapon.recoil();
                if(trans_y < 0){
                    trans_y *= -1;
                }
                Vector3 pos = {transform.position.x, transform.position.y + trans_y, transform.position.z};
                BulletEntity::Create(weapon.weaponType(), weapon.bulletSize(), weapon.damage(), pos, sprite.scale.x>=0.0f? 1 : -1);        
            }

            if(weapon.currentAmmoInClip() == 0){
                if(isProjectile(weapon.weaponType()) || isConsumable(weapon.weaponType())){
                    editSprite(player.currentWeapon, weapon, 0.0f);
                }
            }

            return;
        }

        if (EPSILON_NOT_ZERO(input.Get("reload")))
        {   
            if(weapon.numClips() != 0){
                editSprite(player.currentWeapon, weapon, 1.0f);
                weapon.reload();
            }
            return;
        }

        if (EPSILON_NOT_ZERO(input.Get("cycle_weapons"))) 
        {
            auto num_weapons = player.weapons.size();
            if (num_weapons < 2)
                return;

            player.active_weapon_idx = (player.active_weapon_idx + 1) % num_weapons;
            Weapon wp = player.weapons[player.active_weapon_idx];
            editSprite(player.currentWeapon, wp, 1.0f);
            return;
        }

    });
}

#pragma once
#include "iostream"
#include "core/core.h"
#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"

#include "gameplay/brawler/components/bullet.h"
#include "gameplay/brawler/systems/bullet_system.h"
#include "gameplay/brawler/weapon.h"
#include "gameplay/brawler/components/movable.h"
#include "gameplay/common/simple_collisions.h"

using namespace dagger;

namespace brawler
{

    struct BulletEntity
    {
        Entity entity;
        Bullet& bullet;
        Transform& transform;
        SimpleCollision& simCol;
        Sprite& sprite;

        static BulletEntity Get(Entity entity)
        {
            auto& reg = Engine::Registry();
            auto& bullet = reg.get_or_emplace<Bullet>(entity);
            auto& transform = reg.get_or_emplace<Transform>(entity);
            auto& simCol = reg.get_or_emplace<SimpleCollision>(entity);
            auto& sprite = reg.get_or_emplace<Sprite>(entity);

            return BulletEntity{ entity, bullet, transform, simCol, sprite };
        }

        static BulletEntity Create(
            Entity owner,
            WeaponType weaponType = WeaponType::BANANA,
            int size   = 3,
            int damage = 10,
            Vector2 position_ = { 0, 0 },
            int direction_ = 0
        ) {
            auto& reg = Engine::Registry();
            auto entity = reg.create();

            auto bullet = BulletEntity::Get(entity);

            bullet.bullet.owner = owner;
            bullet.bullet.type = weaponType;

            bullet.transform.position = { position_, 70.0f };

            bullet.sprite.position = { position_, 70.0f };
            bullet.sprite.size = { 1, 1 };
            bullet.bullet.projectile = false;

            switch(weaponType){
                case WeaponType::BAZOOKA:
                    AssignSprite(bullet.sprite, "brawler:bazookaAmmo");
                    break;
                case WeaponType::GRANADE:
                    AssignSprite(bullet.sprite, "brawler:granade");
                    break;
                case WeaponType::MINE:
                    AssignSprite(bullet.sprite, "brawler:mine");
                    break;
                case WeaponType::BANANA:
                    AssignSprite(bullet.sprite, "brawler:banana");
                    break;
                case WeaponType::C4:
                    AssignSprite(bullet.sprite, "brawler:c4");
                    break;
                case WeaponType::MEDKIT:
                    AssignSprite(bullet.sprite, "brawler:medkit");
                    break;
                default:
                    AssignSprite(bullet.sprite, "brawler:bullet");
                    bullet.sprite.scale = { 0.05, 0.05 };
            }
            int dir = direction_ >= 0 ? 1 : -1;
            bullet.bullet.direction = dir;
            bullet.bullet.damage = damage;
            bullet.sprite.scale.x *= dir;

            bullet.simCol.size = {bullet.sprite.size.x*bullet.sprite.scale.x, bullet.sprite.size.y*bullet.sprite.scale.y};

            BulletSystem::s_ActiveBullets++;

            return bullet;
        }
    };

}
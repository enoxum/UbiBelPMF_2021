#pragma once
#include "core/core.h"
#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"

#include "gameplay/brawler/components/weapon_collision.h"
#include "gameplay/brawler/components/bullet.h"
#include "gameplay/brawler/systems/bullet_system.h"
#include "gameplay/brawler/weapon.h"
#include "gameplay/brawler/components/movable.h"

using namespace dagger;

namespace brawler
{

    struct ProjectileEntity
    {
        Entity entity;
        Bullet& bullet;
        Movable& mov;
        Transform& transform;
        WeaponCollision& col;
        Sprite& sprite;
        Animator& animator;

        static ProjectileEntity Get(Entity entity)
        {
            auto& reg = Engine::Registry();
            auto& bullet = reg.get_or_emplace<Bullet>(entity);
            auto& mov = reg.get_or_emplace<Movable>(entity);
            auto& transform = reg.get_or_emplace<Transform>(entity);
            auto& col = reg.get_or_emplace<WeaponCollision>(entity);
            auto& sprite = reg.get_or_emplace<Sprite>(entity);
            auto& animator = reg.get_or_emplace<Animator>(entity);

            return ProjectileEntity{ entity, bullet, mov, transform, col, sprite, animator };
        }

        static ProjectileEntity Create(
            WeaponType weaponType,
            int size   = 3,
            int damage = 10,
            Vector2 position_ = { 0, 0 },
            int direction_ = 0
        ) {
            auto& reg = Engine::Registry();
            auto entity = reg.create();

            auto projectile = ProjectileEntity::Get(entity);

            projectile.mov.speed = {150*direction_, 250};

            projectile.transform.position = { position_, 0.0f };

            projectile.sprite.position = { position_, 0.0f };
            projectile.sprite.size = { 1, 1 };
            projectile.bullet.type = 1;

            switch(weaponType){
                case WeaponType::BAZOOKA:
                    AssignSprite(projectile.sprite, "brawler:bazookaAmmo");
                    break;
                case WeaponType::FLASH:
                    AssignSprite(projectile.sprite, "brawler:flash");
                    break;
                case WeaponType::GRANADE:
                    AssignSprite(projectile.sprite, "brawler:granade");
                    break;
                case WeaponType::MINE:
                    AssignSprite(projectile.sprite, "brawler:mine");
                    break;
                case WeaponType::C4:
                    AssignSprite(projectile.sprite, "brawler:c4");
                    break;
                default:
                    AssignSprite(projectile.sprite, "brawler:bullet");
                    projectile.sprite.scale = { 0.05, 0.05 };
            }

            projectile.bullet.direction = (direction_ >= 0 ? 1 : -1);
            projectile.bullet.damage = damage;

            projectile.col.size = {projectile.sprite.size.x-5, projectile.sprite.size.y-2};

            BulletSystem::s_ActiveBullets++;

            return projectile;
        }
    };

}
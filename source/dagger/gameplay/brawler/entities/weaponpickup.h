#pragma once

#include "core/core.h"
#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include "gameplay/common/simple_collisions.h"
#include "gameplay/brawler/components/weaponpickup.h"

#include "gameplay/brawler/weapon.h"

using namespace dagger;

namespace brawler
{

    struct WeaponPickupEntity
    {
        Entity entity;
        WeaponPickup& weaponPickup;
        Transform& transform;
        Sprite& sprite;
        SimpleCollision& col;

        static WeaponPickupEntity Get(Entity entity)
        {
            auto& reg = Engine::Registry();
            auto& wp = reg.get_or_emplace<WeaponPickup>(entity);
            auto& transform = reg.get_or_emplace<Transform>(entity);
            auto& sprite = reg.get_or_emplace<Sprite>(entity);
            auto& col = reg.get_or_emplace<SimpleCollision>(entity);

            return WeaponPickupEntity{ entity, wp, transform, sprite, col};
        }

        static WeaponPickupEntity Create(
            Vector2 position_ = { 0, 0 },
            const WeaponType& weaponType = WeaponType::PISTOL
        ) {
            auto& reg = Engine::Registry();
            auto entity = reg.create();

            auto wp = WeaponPickupEntity::Get(entity);
            wp.weaponPickup.weapon = Weapon::CreateRandom();
            wp.weaponPickup.spawnTime = Engine::CurrentTime();
            wp.transform.position = { position_.x, position_.y, 80.0f };
            wp.sprite.position = { position_.x, position_.y, 80.0f };
            
            AssignSprite(wp.sprite, "brawler:" + wp.weaponPickup.weapon.sprite());

            wp.col.size = wp.sprite.size;    

            return wp;
        }
    };

}
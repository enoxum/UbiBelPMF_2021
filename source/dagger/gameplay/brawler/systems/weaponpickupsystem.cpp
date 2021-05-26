#include "weaponpickupsystem.h"

#include "iostream"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include "gameplay/common/simple_collisions.h"

#include "gameplay/brawler/components/weaponpickup.h"
#include "gameplay/brawler/entities/weaponpickup.h"
#include "gameplay/brawler/components/player.h"
#include "gameplay/brawler/systems/shooting_system.h"

using namespace brawler;
using namespace dagger;

void WeaponPickupSystem::Run()
{
    auto wpSpriteView = Engine::Registry().view<WeaponPickup, Transform, Sprite>();
    for (auto wpEntity: wpSpriteView)
    {
        auto wpData = WeaponPickupEntity::Get(wpEntity);
        wpData.sprite.position.y = wpData.transform.position.y + std::sin((Engine::CurrentTime() - wpData.weaponPickup.spawnTime).count()/200000000.0f);
    }

    auto wpColView = Engine::Registry().view<WeaponPickup, Transform, SimpleCollision>();
    auto playerColView = Engine::Registry().view<Player, Transform, SimpleCollision>();

    for (auto wpColEntity: wpColView)
    {
        auto wpData = WeaponPickupEntity::Get(wpColEntity);
        auto wpTransform = wpColView.get<Transform>(wpColEntity);
        auto& wpCol = wpData.col;

        for (auto playerColEntity : playerColView)
        {
            const auto& playerCol = playerColView.get<SimpleCollision>(playerColEntity);
            const auto& playerTransform = playerColView.get<Transform>(playerColEntity);
            if (wpCol.IsCollided(wpTransform.position, playerCol, playerTransform.position)) 
            {

                auto& player = playerColView.get<Player>(playerColEntity);
                auto& playerWeapons = player.weapons;
                auto& pickedUpWeapon = wpData.weaponPickup.weapon;

                if (player.pickedUpWeapons == player.maxWeapons)
                    continue;

                auto existingWeaponIt
                    = std::find_if(std::begin(playerWeapons),
                        std::end(playerWeapons),
                        [&pickedUpWeapon](const auto& w) {
                            return w.weaponType() == pickedUpWeapon.weaponType();
                        });
                if (existingWeaponIt == std::end(playerWeapons)) {
                    playerWeapons.push_back(pickedUpWeapon);
                    player.pickedUpWeapons++;
                    if (playerWeapons.size() == 1) {
                        player.active_weapon_idx = 0;

                        auto& weaponSprite = Engine::Registry().get<Sprite>(player.currentWeapon);
                        int dir = weaponSprite.scale.x >= 0.0f ? 1 : -1;
                        weaponSprite.position.x += pickedUpWeapon.translate().x * dir;
                        weaponSprite.position.y += pickedUpWeapon.translate().y;
                        ShootingSystem::editSprite(player.currentWeapon, pickedUpWeapon, 1.0f);
                    }
                } else {
                    existingWeaponIt->transferAmmo(pickedUpWeapon);
                }
                Engine::Registry().destroy(wpColEntity);
            }
        }
    }
}
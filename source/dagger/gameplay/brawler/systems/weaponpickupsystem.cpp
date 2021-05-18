#include "weaponpickupsystem.h"

#include "iostream"

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include "gameplay/common/simple_collisions.h"

#include "gameplay/brawler/components/weaponpickup.h"
#include "gameplay/brawler/entities/weaponpickup.h"
#include "gameplay/brawler/systems/bullet_system.h"
#include "gameplay/brawler/components/player.h"
#include "gameplay/brawler/systems/shooting_system.h"

using namespace brawler;
using namespace dagger;

void WeaponPickupSystem::Run()
{

    auto viewInputDebug = Engine::Registry().view<InputReceiver>();

    viewInputDebug.each([](auto& input) {
        if (EPSILON_NOT_ZERO(input.Get("spawn_weapon_debug")))
        {
            // TODO: This will need to include platforms as well
            // TODO: Also need to check whether pickups overlap with each other
            // TODO: Generate between camera bounds instead of hardcode
            // That's a lot of TODOs :D
            auto rand_x = static_cast<float>((rand() % (175 + 1 + 175)));
            WeaponPickupEntity::Create(Vector2{ rand_x, 60.0f}, static_cast<WeaponType>(rand() % 3));
        }
    });

    auto wpColView = Engine::Registry().view<WeaponPickup, Transform, SimpleCollision>();
    auto playerColView = Engine::Registry().view<Player, Transform, SimpleCollision>();

    auto wpColIt = wpColView.begin();

   
    for (auto wpColEntity: wpColView)
    {
        auto wpData = WeaponPickupEntity::Get(wpColEntity);
        auto& wpCol = wpData.col;

        if (wpData.weaponPickup.pickedUp || !wpCol.colided)
            continue;
        for (auto playerColEntity : playerColView)
        {
            const auto& playerCol = playerColView.get<SimpleCollision>(playerColEntity);
            if (!playerCol.colided || playerCol.colidedWith != wpColEntity)
                continue;

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
                    int dir = weaponSprite.scale.x>=0.0f? 1 : -1;
                    weaponSprite.position.x += pickedUpWeapon.translate().x * dir;
                    weaponSprite.position.y += pickedUpWeapon.translate().y;
                    ShootingSystem::editSprite(player.currentWeapon, pickedUpWeapon, 1.0f);
                }
            }
            else {
                existingWeaponIt->transferAmmo(pickedUpWeapon);
            }

            wpData.weaponPickup.pickedUp = true;
        }
    }

    for (auto wpColEntity : wpColView)
    {
        if (WeaponPickupEntity::Get(wpColEntity).weaponPickup.pickedUp)
        {
            Engine::Registry().destroy(wpColEntity);
        }
    }
}
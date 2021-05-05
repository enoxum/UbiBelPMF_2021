#include "weaponpickupsystem.h"

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include "gameplay/common/simple_collisions.h"

#include "gameplay/brawler/components/weaponpickup.h"
#include "gameplay/brawler/entities/weaponpickup.h"
#include "gameplay/brawler/systems/bullet_system.h"
#include "gameplay/brawler/components/player.h"

#include <iostream>

using namespace brawler;
using namespace dagger;

void WeaponPickupSystem::Run()
{

    auto viewInputDebug = Engine::Registry().view<InputReceiver>();

    viewInputDebug.each([](auto& input) {
        if (EPSILON_NOT_ZERO(input.Get("spawn_weapon_debug")))
        {
            // TODO: This will need to include platforms as well
            auto rand_x = static_cast<float>((rand() % (175 + 1 + 175)) - 175);
            WeaponPickupEntity::Create(Vector2{ rand_x, 0.0f}, static_cast<WeaponType>(rand() % 3));
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
            const WeaponType& weapon = wpData.weaponPickup.weaponType;
            auto w = std::find(playerWeapons.begin(), playerWeapons.end(), weapon);
            if (w != playerWeapons.end()) {
                int idx = w - playerWeapons.begin();
                player.ammo[idx] += 100;
            }
            else {
                playerWeapons.push_back(weapon);
                player.ammo.push_back(100);
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
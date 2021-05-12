#include "weapon_spawn_system.h"

#include "core/game/transforms.h"

#include "gameplay/brawler/entities/weaponpickup.h"
#include "gameplay/brawler/components/weapon_spawner.h"

using namespace dagger;
using namespace brawler;

void WeaponSpawnSystem::Run()
{
    auto& reg = Engine::Registry();

    auto objects = reg.view< WeaponSpawner>();
    for (auto object : objects)
    {
        auto& s = reg.get<WeaponSpawner>(object);

        if(!reg.valid(s.weapon))
        {
            if(s.cooldown > 0)
            {
                s.cooldown -= Engine::DeltaTime();
            }
            else
            {
                auto weapon = WeaponPickupEntity::Create(s.position);
                s.weapon = weapon.entity;
                s.cooldown = TOTAL_COOLDOWN;
            }
        }
    }
}
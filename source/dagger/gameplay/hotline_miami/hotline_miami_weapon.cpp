#include "hotline_miami_weapon.h"
#include "gameplay/hotline_miami/hotline_miami_player.h"
#include "gameplay/hotline_miami/hotline_miami_tools.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include <algorithm>
#include <execution>

#include "gameplay/common/simple_collisions.h"


using namespace dagger;
using namespace hotline_miami;

void HotlineMiamiWeaponSystem::Run()
{
    auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision, HotlineMiamiPlayer>();
    auto view = Engine::Registry().view<Transform, SimpleCollision, HotlineMiamiWeapon>();
    for (auto entity : view)
    {
        auto& col = view.get<SimpleCollision>(entity);
        auto& weapon = view.get<HotlineMiamiWeapon>(entity);

        if (col.colided)
        {
            if (Engine::Registry().valid(col.colidedWith) && viewCollisions.contains(col.colidedWith))
            {
                auto& player = viewCollisions.get<HotlineMiamiPlayer>(col.colidedWith);
                player.weapon_type = weapon.type;
                Engine::Registry().emplace<HotlineMiamiDeleteEntity>(entity);
            }
            col.colided = false;
        }
    }
}

void HotlineMiamiKeySystem::Run()
{
    auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision, HotlineMiamiPlayer>();
    auto view = Engine::Registry().view<Transform, SimpleCollision, HotlineMiamiKey>();
    for (auto entity : view)
    {
        auto& col = view.get<SimpleCollision>(entity);
        auto& key = view.get<HotlineMiamiKey>(entity);

        if (col.colided)
        {
            if (Engine::Registry().valid(col.colidedWith) && viewCollisions.contains(col.colidedWith))
            {
                auto& player = viewCollisions.get<HotlineMiamiPlayer>(col.colidedWith);
                player.has_key = true;
                Engine::Registry().emplace<HotlineMiamiDeleteEntity>(entity);
            }
            col.colided = false;
        }
    }
}

void HotlineMiamiWorldEndSystem::Run()
{
    auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision, HotlineMiamiPlayer>();
    auto view = Engine::Registry().view<Transform, SimpleCollision, HotlineMiamiWorldEnd>();
    for (auto entity : view)
    {
        auto& col = view.get<SimpleCollision>(entity);
        auto& key = view.get<HotlineMiamiWorldEnd>(entity);

        if (col.colided)
        {
            if (Engine::Registry().valid(col.colidedWith) && viewCollisions.contains(col.colidedWith))
            {
                auto& player = viewCollisions.get<HotlineMiamiPlayer>(col.colidedWith);
                if (player.has_key)
                {
                    Engine::Registry().emplace<HotlineMiamiDeleteEntity>(entity);
                }
;            }
            col.colided = false;
        }
    }
}
#include "hotline_miami_projectile.h"
#include "gameplay/hotline_miami/hotline_miami_obstacle.h"
#include "gameplay/hotline_miami/hotline_miami_tools.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include "gameplay/common/simple_collisions.h"

#include <algorithm>
#include <execution>


using namespace dagger;
using namespace hotline_miami;

void HotlineMiamiProjectileSystem::Run()
{
    auto view = Engine::Registry().view<HotlineMiamiProjectile, Transform>();
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& projectile = view.get<HotlineMiamiProjectile>(entity);
        t.position += projectile.direction * projectile.projectile_speed * Engine::DeltaTime();
    }
}

void HotlineMiamiProjectileObstacleCollisionSystem::Run()
{
    auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision, HotlineMiamiObstacle>();
    auto view = Engine::Registry().view<Transform, SimpleCollision, HotlineMiamiProjectile>();
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& col = view.get<SimpleCollision>(entity);
        auto& projectile = view.get<HotlineMiamiProjectile>(entity);

        if (col.colided)
        {
            if (Engine::Registry().valid(col.colidedWith))
            {
                Engine::Registry().emplace<HotlineMiamiDeleteEntity>(entity);
            }
        }
    }
}
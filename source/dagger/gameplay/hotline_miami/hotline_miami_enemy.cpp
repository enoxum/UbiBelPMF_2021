#include "hotline_miami_enemy.h"
#include "gameplay/hotline_miami/hotline_miami_projectile.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "gameplay/hotline_miami/hotline_miami_tools.h"

#include "gameplay/common/simple_collisions.h"

#include <algorithm>
#include <execution>
#include <gameplay/hotline_miami/hotline_miami_obstacle.h>
#include <gameplay/hotline_miami/hotline_miami_player.h>



using namespace dagger;
using namespace hotline_miami;

void HotlineMiamiEnemyProjectileCollisionSystem::Run()
{
    auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision, HotlineMiamiProjectile>();
    auto view = Engine::Registry().view<Transform, SimpleCollision, HotlineMiamiEnemy>();
    for (auto entity : view)
    {
        auto& col = view.get<SimpleCollision>(entity);

        if (col.colided)
        {
            if (Engine::Registry().valid(col.colidedWith) && viewCollisions.contains(col.colidedWith))
            {
                Engine::Registry().emplace<HotlineMiamiDeleteEntity>(entity);
                Engine::Registry().emplace<HotlineMiamiDeleteEntity>(col.colidedWith);
            }
            col.colided = false;
        }
    }
}

void HotlineMiamiEnemyBulletSystem::Run()
{   
    if (interval-- == 0) {
        interval = 500;
        auto enemies = Engine::Registry().view<Transform, HotlineMiamiEnemy>();
        for (auto entity : enemies)
        {
            auto& t = enemies.get<Transform>(entity);
            auto& enemie = enemies.get<HotlineMiamiEnemy>(entity);

            // make a bullet
            {
                float tileSize = 20.f;

                auto& engine = Engine::Instance();
                auto& reg = engine.Registry();
                auto entity = reg.create();

                auto& collision = reg.emplace<SimpleCollision>(entity);
                collision.size.x = tileSize;
                collision.size.y = tileSize;

                auto& transform = reg.emplace<Transform>(entity);
                transform.position = t.position;

                auto& sprite = reg.emplace<Sprite>(entity);

                AssignSprite(sprite, "hotline_miami:Projectile:projectile_pistol");
                sprite.size.x = tileSize;
                sprite.size.y = tileSize;

                auto& bullet = reg.emplace<HotlineMiamiEnemyBullet>(entity);
            }
        }
    }
    auto view1 = Engine::Registry().view<Transform, HotlineMiamiPlayer>();
    auto view2 = Engine::Registry().view<Transform, HotlineMiamiEnemyBullet>();
    for (auto entity : view1)
    {
        auto& t_player = view1.get<Transform>(entity);
        auto& player = view1.get<HotlineMiamiPlayer>(entity);

        for (auto entity : view2)
        {
            auto& t = view2.get<Transform>(entity);
            auto& bullet = view2.get<HotlineMiamiEnemyBullet>(entity);
            if (!bullet.has_direction) 
            {
                SetupEnemyBulletStats(bullet, Vector3(-1, 0, 0));
                bullet.has_direction = true;
            }
            t.position += bullet.direction * bullet.bullet_speed * Engine::DeltaTime();
        }
    }
}

void HotlineMiamiEnemyBulletObstacleCollisionSystem::Run() 
{
    auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision, HotlineMiamiObstacle>();
    auto view = Engine::Registry().view<Transform, SimpleCollision, HotlineMiamiEnemyBullet>();
    for (auto entity : view)
    {
        auto& col = view.get<SimpleCollision>(entity);

        if (col.colided)
        {
            if (Engine::Registry().valid(col.colidedWith) && viewCollisions.contains(col.colidedWith))
            {
                Engine::Registry().emplace<HotlineMiamiDeleteEntity>(entity);
            }
            col.colided = false;
        }
    }
}

void HotlineMiamiEnemyBulletPlayerCollisionSystem::Run()
{
    auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision, HotlineMiamiPlayer>();
    auto view = Engine::Registry().view<Transform, SimpleCollision, HotlineMiamiEnemyBullet>();
    for (auto entity : view)
    {
        auto& col = view.get<SimpleCollision>(entity);

        if (col.colided)
        {
            if (Engine::Registry().valid(col.colidedWith) && viewCollisions.contains(col.colidedWith))
            {
                Engine::Registry().emplace<HotlineMiamiDeleteEntity>(entity);
            }
            col.colided = false;
        }
    }
}
#include "hotline_miami_enemy.h"
#include "gameplay/hotline_miami/hotline_miami_projectile.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "gameplay/hotline_miami/hotline_miami_tools.h"

#include "gameplay/common/simple_collisions.h"
#include "math.h"

#include <algorithm>
#include <execution>
#include <gameplay/hotline_miami/hotline_miami_obstacle.h>
#include <gameplay/hotline_miami/hotline_miami_player.h>



using namespace dagger;
using namespace hotline_miami;


void HotlineMiamiEnemyProjectileCollisionSystem::Run()
{
    auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision, HotlineMiamiProjectile>();
    auto view = Engine::Registry().view<Transform, SimpleCollision, Sprite, HotlineMiamiEnemy>();
    for (auto entity : view)
    {
        auto& col = view.get<SimpleCollision>(entity);
        auto& sprite = view.get<Sprite>(entity);
        auto& enemy = view.get<HotlineMiamiEnemy>(entity);

        if (col.colided)
        {
            if (Engine::Registry().valid(col.colidedWith) && viewCollisions.contains(col.colidedWith))
            {
                enemy.is_dead = true;
                AssignSprite(sprite, "hotline_miami:Enemy:enemy_dead");
                Engine::Registry().emplace<HotlineMiamiDeleteEntity>(col.colidedWith);
                Engine::Registry().remove<SimpleCollision>(entity);
            }   
            col.colided = false;
        }
    }
}

void HotlineMiamiEnemyBulletSystem::Run()
{   
    if (interval-- == 0) {
        interval = 10;
        auto players = Engine::Registry().view<Transform, HotlineMiamiPlayer>();
        auto enemies = Engine::Registry().view<Transform, Sprite, HotlineMiamiEnemy>();
        for (auto entity : players)
        {
            auto& t_player = players.get<Transform>(entity);
            auto& player = players.get<HotlineMiamiPlayer>(entity);

            for (auto entity : enemies)
            {
                auto& t = enemies.get<Transform>(entity);
                auto& enemy_sprite = enemies.get<Sprite>(entity);
                auto& enemy = enemies.get<HotlineMiamiEnemy>(entity);

                // make a bullet
                if (!enemy.is_dead)
                {
                    float tileSize = 8.f;

                    auto& engine = Engine::Instance();
                    auto& reg = engine.Registry();
                    auto entity = reg.create();

                    auto& collision = reg.emplace<SimpleCollision>(entity);
                    collision.size.x = tileSize;
                    collision.size.y = tileSize;

                    auto& transform = reg.emplace<Transform>(entity);
                    transform.position = t.position;

                    auto& sprite = reg.emplace<Sprite>(entity);

                    AssignSprite(sprite, "hotline_miami:Projectile:enemy_bullet");
                    sprite.size.x = tileSize;
                    sprite.size.y = tileSize;

                    auto& bullet = reg.emplace<HotlineMiamiEnemyBullet>(entity);
                    Float32 x = (t_player.position.x - t.position.x) / (abs(t_player.position.x - t.position.x) + abs(t_player.position.y - t.position.y));
                    Float32 y = (t_player.position.y - t.position.y) / (abs(t_player.position.x - t.position.x) + abs(t_player.position.y - t.position.y));
                    SetupEnemyBulletStats(bullet, Vector3(x, y, 0));

                    enemy_sprite.rotation = (atan2(y, x) * 180.f) / 3.14 + 90;
                }
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
        }
    }
}
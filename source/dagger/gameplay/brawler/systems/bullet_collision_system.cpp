#include "bullet_collision_system.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include "gameplay/brawler/level.h"
#include "gameplay/brawler/components/player.h"
#include "gameplay/brawler/components/movable.h"
#include "gameplay/brawler/components/tile.h"

using namespace brawler;
using namespace dagger;

void BulletCollisionSystem::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&BulletCollisionSystem::OnFrameEnd>(this);
}

void BulletCollisionSystem::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&BulletCollisionSystem::OnFrameEnd>(this);
}

void BulletCollisionSystem::Run()
{
    auto bullets = Engine::Registry().view<Bullet, Transform, SimpleCollision>();

    for(auto bullet : bullets){

        auto& b = bullets.get<Bullet>(bullet);

        if(b.projectile)
            continue;

        auto& collision = bullets.get<SimpleCollision>(bullet);
        auto& transform = bullets.get<Transform>(bullet);

        auto players = Engine::Registry().view<SimpleCollision, Transform, Movable, Player>();

        for(auto player : players){

            if(b.owner == player)
                continue;

            auto &col = players.get<SimpleCollision>(player);
            auto &tr = players.get<Transform>(player);
            auto &mov = players.get<Movable>(player);
            
            if (collision.IsCollided(transform.position, col, tr.position))
            {
                auto& p = players.get<Player>(player);
                bool dead = p.dealDamage(b.damage);
                mov.speed.x += b.direction*b.damage;
                b.shouldDestroy = true;
                break;
            }
        }

        if (b.shouldDestroy)
            continue;

        auto tiles = Engine::Registry().view<SimpleCollision, Tile, Transform>();

        for(auto tile : tiles){

            auto& col = tiles.get<SimpleCollision>(tile);
            auto& tr = tiles.get<Transform>(tile);

            if (collision.IsCollided(transform.position, col, tr.position))
            {
                b.shouldDestroy = true;
                break;
            }
        }
    }
}

void BulletCollisionSystem::OnFrameEnd()
{
    Engine::Registry().view<Bullet>().each([&](Entity entity, const Bullet& b)
        {
            if (b.shouldDestroy)
            {
                Engine::Registry().destroy(entity);
                BulletSystem::s_ActiveBullets--;
            }
        });
}
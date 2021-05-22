#include "bullet_collision_system.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include "gameplay/brawler/level.h"
#include "gameplay/brawler/components/player.h"
#include "gameplay/brawler/components/movable.h"
#include "gameplay/brawler/components/weapon_collision.h"
#include "gameplay/brawler/components/tile.h"

#include "iostream"

using namespace brawler;
using namespace dagger;

void BulletCollisionSystem::Run()
{
    // Dohvataju se metci i projektili
    auto bullets = Engine::Registry().view<Bullet, Transform, WeaponCollision, SimpleCollision>();
    // Dohvataju se igraci
    auto players = Engine::Registry().view<SimpleCollision, Transform, Movable, Player>();
    // Dohvatanje tile-ova
    auto tiles = Engine::Registry().view<SimpleCollision, Tile, Transform>();

    for(auto bullet : bullets){

        auto& b = bullets.get<Bullet>(bullet);

        if(b.projectile)
            continue;

        auto& collision = bullets.get<SimpleCollision>(bullet);
        auto& transform = bullets.get<Transform>(bullet);

        for(auto player : players){

            auto &col = players.get<SimpleCollision>(player);
            auto &tr = players.get<Transform>(player);
            auto &mov = players.get<Movable>(player);
            
            if(b.owner == player)
                continue;

            //processing one collision per frame for each colider
            if (collision.IsCollided(transform.position, col, tr.position))
            {
                // Logic
                // TODO Reduce health
                mov.speed.x += b.direction*b.damage;
                Engine::Registry().destroy(bullet);
                break;
            }
        }

        for(auto tile : tiles){

            auto& col = tiles.get<SimpleCollision>(tile);
            auto& tr = tiles.get<Transform>(tile);

            if (collision.IsCollided(transform.position, col, tr.position))
            {
                Engine::Registry().destroy(bullet);
                break;
            }
        }
    }
}

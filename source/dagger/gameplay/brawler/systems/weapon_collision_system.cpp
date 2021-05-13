#include "weapon_collision_system.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include "gameplay/brawler/level.h"
#include "gameplay/brawler/components/player.h"
#include "gameplay/brawler/components/movable.h"
#include "gameplay/brawler/components/weapon_collision.h"

using namespace brawler;
using namespace dagger;

void WeaponCollisionSystem::Run()
{
    // Dohvataju se metci i projektili
    auto bullets = Engine::Registry().view<Bullet, Transform, WeaponCollision>();
    // Dohvataju se igraci
    auto players = Engine::Registry().view<SimpleCollision, Transform, Movable>();

    for(auto bullet : bullets){

        auto& b = bullets.get<Bullet>(bullet);
        auto& collision = bullets.get<WeaponCollision>(bullet);
        auto& transform = bullets.get<Transform>(bullet);

        for(auto player : players){
            
            // Check if the bullet belongs to this player
            // If thats true just skip

            auto &col = players.get<SimpleCollision>(player);
            auto &tr = players.get<Transform>(player);
            
            //processing one collision per frame for each colider
            if (collision.IsCollided(transform.position, col, tr.position))
            {
                collision.colided = true;
                collision.colidedWith = player;

                col.colided = true;
                col.colidedWith = bullet;
            }
        }
    }
}

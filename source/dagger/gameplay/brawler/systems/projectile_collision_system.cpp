#include "projectile_collision_system.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include "gameplay/brawler/level.h"
#include "gameplay/brawler/components/player.h"
#include "gameplay/brawler/components/movable.h"
#include "gameplay/brawler/components/tile.h"

using namespace brawler;
using namespace dagger;

void ProjectileCollisionSystem::Run()
{
    auto bullets = Engine::Registry().view<Bullet, Transform, SimpleCollision, Animator, Movable>();
    auto players = Engine::Registry().view<SimpleCollision, Transform, Movable, Player>();
    auto tiles = Engine::Registry().view<SimpleCollision, Tile, Transform>();

    for(auto bullet : bullets){

        auto& b = bullets.get<Bullet>(bullet);

        if(!b.projectile)
            continue;

        auto& collision = bullets.get<SimpleCollision>(bullet);
        auto& transform = bullets.get<Transform>(bullet);
        auto& movable = bullets.get<Movable>(bullet);
        auto& animator = bullets.get<Animator>(bullet);

        // Waiting for merge
        if(animator.currentFrame == 24){
            Engine::Registry().destroy(bullet);
            BulletSystem::s_ActiveBullets--;
            continue;
        }

        switch (b.type)
        {
        case WeaponType::C4:
            if(b.timer){
                b.duration -= Engine::DeltaTime();
                if(b.duration < 0){
                    b.timer = false;
                    AnimatorPlay(animator, "EXPLOSION");
                }
            }
            break;
        case WeaponType::MINE:

            for(auto player : players){
                auto &col = players.get<SimpleCollision>(player);
                auto &tr = players.get<Transform>(player);
                auto &mov = players.get<Movable>(player);
                
                if(b.owner == player)
                    continue;

                //processing one collision per frame for each colider
                if (collision.IsCollided(transform.position, col, tr.position))
                {
                    collision.colided = true;
                    AnimatorPlay(animator, "EXPLOSION");

                    auto vec = transform.position - tr.position;

                    mov.speed -= Vector2{vec.x*50, vec.y*50};
                    // TODO Reduce health
                    break;
                }
            }   
            break;
        case WeaponType::GRANADE:
            if(movable.isOnGround){
                AnimatorPlay(animator, "EXPLOSION");
            }
            break;
        default:
            break;
        }
    }
}
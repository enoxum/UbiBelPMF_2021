#include "projectile_collision_system.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include "gameplay/brawler/level.h"
#include "gameplay/brawler/components/player.h"
#include "gameplay/brawler/components/movable.h"
#include "gameplay/brawler/components/tile.h"

using namespace brawler;
using namespace dagger;

float ProjectileCollisionSystem::s_grenadeSquaredRange = 200.0f;
float ProjectileCollisionSystem::s_c4SquaredRange = 300.0f;




float ProjectileCollisionSystem::getDamageCoeff(const Vector3& playerPosition, const Vector3& projectilePosition, float range)
{
    float dist = (playerPosition.x - projectilePosition.x) * (playerPosition.x - projectilePosition.x) + (playerPosition.y - projectilePosition.y) * (playerPosition.y - projectilePosition.y);
    if (dist > range)
        return 0.0f;

    return 1/dist;
    
}


bool ProjectileCollisionSystem::explodePlayer(Player& player,
                                              Movable& playerMovable,
                                              Bullet& bullet,
                                              const Vector3& playerPosition,
                                              const Vector3& bulletPosition,
                                              float bulletSquareRange)
{
    float damage_coeff = getDamageCoeff(playerPosition, bulletPosition, bulletSquareRange);
    if (damage_coeff == 0) {
        return false;
    }

    auto vec = bulletPosition - playerPosition;

    playerMovable.speed -= Vector2{ vec.x * 50, vec.y * 50 };
    bool dead = player.dealDamage(100* damage_coeff * bullet.damage);

    return dead;

}

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
                    for (auto affectedPlayerEntity : players)
                    {
                        explodePlayer(players.get<Player>(affectedPlayerEntity),
                            players.get<Movable>(affectedPlayerEntity), 
                            b, 
                            players.get<Transform>(affectedPlayerEntity).position, 
                            transform.position, ProjectileCollisionSystem::s_c4SquaredRange);
                    }

                }
            }
            break;
        case WeaponType::MINE:

            for(auto player : players){
                auto &col = players.get<SimpleCollision>(player);
                auto &tr = players.get<Transform>(player);
                auto &mov = players.get<Movable>(player);
                auto &p = players.get<Player>(player);
                
                if(b.owner == player)
                    continue;

                //processing one collision per frame for each colider
                if (collision.IsCollided(transform.position, col, tr.position))
                {
                    collision.colided = true;
                    AnimatorPlay(animator, "EXPLOSION");

                    auto vec = transform.position - tr.position;

                    mov.speed -= Vector2{vec.x*50, vec.y*50};
                    bool dead = p.dealDamage(b.damage);
                    break;
                }
            }   
            break;
        case WeaponType::GRANADE:
            if(movable.isOnGround){
                AnimatorPlay(animator, "EXPLOSION");

                for (auto affectedPlayerEntity : players)
                {
                    explodePlayer(players.get<Player>(affectedPlayerEntity),
                        players.get<Movable>(affectedPlayerEntity),
                        b,
                        players.get<Transform>(affectedPlayerEntity).position,
                        transform.position, 
                        ProjectileCollisionSystem::s_grenadeSquaredRange);
                }

            }
            break;
        default:
            break;
        }
    }
}

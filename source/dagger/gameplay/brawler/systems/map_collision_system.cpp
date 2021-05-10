#include "map_collision_system.h"

#include "core/game/transforms.h"
#include "gameplay/brawler/components/movable.h"
#include "gameplay/common/simple_collisions.h"
#include "gameplay/brawler/level.h"

using namespace dagger;
using namespace brawler;

void MapCollisionSystem::Run()
{
    auto objects = Engine::Registry().view<Transform, Movable, SimpleCollision>();
    for (auto obj : objects)
    {
        auto& t = objects.get<Transform>(obj);
        auto& m = objects.get<Movable>(obj);
        auto& c = objects.get<SimpleCollision>(obj);

        auto leftWall = Level::getLeftWall(t, m, c);
        if(leftWall && m.prevPosition.x > t.position.x && m.prevPosition.x >= leftWall.value()) {
            t.position.x = leftWall.value();
            m.speed.x = 0.0f;
        }

        auto rightWall = Level::getRightWall(t, m, c);
        if(rightWall && m.prevPosition.x < t.position.x && m.prevPosition.x <= rightWall.value()) {
            t.position.x = rightWall.value();
            m.speed.x = 0.0f;
        }

        auto ground = Level::getGround(t, m, c);
        if(ground && m.speed.y<=0) {
            t.position.y = ground.value();
            m.speed.y = 0;
            m.isOnGround = true;
        } else {
            m.isOnGround = false;
        }

        auto ceiling = Level::getCeiling(t, m, c);
        if(ceiling && m.speed.y>0.0f) {
            t.position.y = ceiling.value()-1;
            m.speed.y = 0;
        }
    }
}
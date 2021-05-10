#include "physics.h"

#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include "gameplay/brawler/components/movable.h"
#include "gameplay/brawler/level.h"

using namespace dagger;
using namespace brawler;

bool PhysicsSystem::s_UseGravity = true;
float PhysicsSystem::s_Gravity = 500.0f;
float PhysicsSystem::s_RunSpeed = 150.0f;
float PhysicsSystem::s_JumpSpeed = 500.0f;
float PhysicsSystem::s_TerminalVelocity = 800.0f;
float PhysicsSystem::s_DragSpeed = 100.0f;
float PhysicsSystem::s_AirMobility = 0.9f;

void PhysicsSystem::Run()
{
    auto objects = Engine::Registry().view<Transform, Movable, SimpleCollision>();
    for (auto obj : objects)
    {
        auto& t = objects.get<Transform>(obj);
        auto& m = objects.get<Movable>(obj);
        auto& c = objects.get<SimpleCollision>(obj);

        // Drag
        if(EPSILON_ZERO(m.speed.x)) {
            m.speed.x = 0;
        } else {
            m.speed.x -= (m.speed.x < 0.0f? -1.0f : 1.0f) * PhysicsSystem::s_DragSpeed * Engine::DeltaTime();
            if(m.prevSpeed.x * m.speed.x < 0)
                m.speed.x = 0;
        }

        // Gravity
        if(s_UseGravity && !m.isOnGround)
            m.speed.y -= PhysicsSystem::s_Gravity * Engine::DeltaTime();

        // Clamp speed
        if(m.speed.y > s_TerminalVelocity)
            m.speed.y = s_TerminalVelocity;
        if(m.speed.y < -s_TerminalVelocity)
            m.speed.y = -s_TerminalVelocity;

        // Update position
        t.position.x += m.speed.x * Engine::DeltaTime();
        t.position.y += m.speed.y * Engine::DeltaTime();

        // Tilemap collision
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

        // Update previous frame data
        m.prevPosition = t.position;
        m.prevSpeed = m.speed;

    }
}

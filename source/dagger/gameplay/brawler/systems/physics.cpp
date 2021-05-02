#include "physics.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "../components/movable.h"
#include "core/graphics/sprite.h"

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
    auto objects = Engine::Registry().view<Transform, Movable, Sprite>();
    for (auto obj : objects)
    {
        auto& t = objects.get<Transform>(obj);
        auto& m = objects.get<Movable>(obj);
        auto& s = objects.get<Sprite>(obj);

        m.prevPosition = t.position;
        m.prevSpeed = m.speed;
        m.wasOnGround = m.isOnGround;

        // Drag
        if(EPSILON_ZERO(m.speed.x)) {
            m.speed.x = 0;
        } else {
            m.speed.x -= (m.speed.x < 0.0f? -1.0f : 1.0f) * PhysicsSystem::s_DragSpeed * Engine::DeltaTime();
            if(m.prevSpeed.x * m.speed.x < 0)
                m.speed.x = 0;
        }

        // Gravity
        if(s_UseGravity)
            m.speed.y -= PhysicsSystem::s_Gravity * Engine::DeltaTime();

        // Clamp speed
        if(m.speed.y > s_TerminalVelocity)
            m.speed.y = s_TerminalVelocity;
        if(m.speed.y < -s_TerminalVelocity)
            m.speed.y = -s_TerminalVelocity;

        // Update position
        t.position.x += m.speed.x * Engine::DeltaTime();
        t.position.y += m.speed.y * Engine::DeltaTime();

        // Simple y=0 platform collision
        if (t.position.y < 0)
        {
            t.position.y = 0;
            m.isOnGround = true;
            if(m.speed.y < 0)
                m.speed.y = 0;
        }
        else
        {
            m.isOnGround = false;
        }

    }
}

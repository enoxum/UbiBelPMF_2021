#include "debug_gui.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include "gameplay/brawler/components/movable.h"
#include "gameplay/brawler/entities/character.h"
#include "gameplay/brawler/systems/physics.h"
#include "gameplay/brawler/systems/bullet_system.h"

using namespace dagger;
using namespace brawler;

void DebugGui::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&DebugGui::OnEndOfFrame>(this);

    Engine::Dispatcher().sink<ToolMenuRender>().connect<&DebugGui::RenderToolMenu>(this);
}

void DebugGui::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&DebugGui::OnEndOfFrame>(this);

    Engine::Dispatcher().sink<ToolMenuRender>().disconnect<&DebugGui::RenderToolMenu>(this);
}

void DebugGui::RenderToolMenu()
{
    // Menu
    if (ImGui::BeginMenu("Brawler"))
    {
        if (ImGui::MenuItem("Reset Position"))
        {
            resetPosition = true;
        }

        ImGui::EndMenu();
    }

    // Window
    ImGui::Begin("Brawler Debug", &debugActive);

    if (ImGui::Button("Reposition Players", ImVec2(ImGui::GetWindowSize().x, 0.0f)))
    {
        resetPosition = true;
    }
    Engine::Registry().view<Transform, Movable, Player>().each([&](Transform& t, Movable& m, Player& p)
        {
            ImGui::Text("Position: %f, %f", t.position.x, t.position.y);
            ImGui::Text("Speed: %f, %f", m.speed.x, m.speed.y);
            ImGui::Separator();
        });

    // Physics
    ImGui::Text("Physics");
    ImGui::Checkbox("Use Gravity", &PhysicsSystem::s_UseGravity);
    ImGui::SliderFloat("Gravity", &PhysicsSystem::s_Gravity, 0.0f, 1000.0f);
    ImGui::SliderFloat("Drag Speed", &PhysicsSystem::s_DragSpeed, 0.0f, 200.0f);

    ImGui::Separator();

    // Movement
    ImGui::Text("Movement");
    ImGui::SliderFloat("Run Speed", &PhysicsSystem::s_RunSpeed, 0.0f, 400.0f);
    ImGui::SliderFloat("Jump Speed", &PhysicsSystem::s_JumpSpeed, 0.0f, 800.0f);
    ImGui::SliderFloat("Terminal Velocity", &PhysicsSystem::s_TerminalVelocity, 0.0f, 1000.0f);
    ImGui::SliderFloat("Air Mobility", &PhysicsSystem::s_AirMobility, 0.0f, 1.0f);

    ImGui::Separator();

    // Shooting
    // std::stringstream ss;
    ImGui::Text("Active bullets: %d", BulletSystem::activeBullets);
    ImGui::SliderFloat("Bullet speed", &BulletSystem::bulletSpeed, 0.0f, 800.0f);

    ImGui::Separator();
    ImGui::Text("Cam bound left: %f", BulletSystem::cameraBoundLeft);
    ImGui::Text("Cam bound right: %f", BulletSystem::cameraBoundRight);
    ImGui::Text("Cam bound up: %f", BulletSystem::cameraBoundUp);
    ImGui::Text("Cam bound down: %f", BulletSystem::cameraBoundDown);
     
    ImGui::End();
}

void DebugGui::Run()
{
}

void DebugGui::OnEndOfFrame()
{
    if(resetPosition) {
        Engine::Registry().view<Transform, Movable, Player>().each([&](Transform& t, Movable& m, Player& p)
            {
                t.position.x = 0.0f;
                t.position.y = 0.0f;
                m.speed.x = 0.0f;
                m.speed.y = 0.0f;
            });
        resetPosition = false;
    }
}
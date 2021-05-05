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
    Engine::Dispatcher().sink<GUIRender>().connect<&DebugGui::RenderDebugWindow>(this);
}

void DebugGui::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&DebugGui::OnEndOfFrame>(this);

    Engine::Dispatcher().sink<ToolMenuRender>().disconnect<&DebugGui::RenderToolMenu>(this);
    Engine::Dispatcher().sink<GUIRender>().disconnect<&DebugGui::RenderDebugWindow>(this);
}

void DebugGui::RenderToolMenu()
{
    if (ImGui::BeginMenu("Brawler"))
    {
        if (ImGui::MenuItem("Reset Position"))
        {
            resetPosition = true;
        }

        ImGui::EndMenu();
    }
}

void DebugGui::RenderDebugWindow()
{
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
    if(ImGui::CollapsingHeader("Physics"))
    {
        ImGui::Checkbox("Use Gravity", &PhysicsSystem::s_UseGravity);
        ImGui::SliderFloat("Gravity", &PhysicsSystem::s_Gravity, 0.0f, 1000.0f);
        ImGui::SliderFloat("Drag Speed", &PhysicsSystem::s_DragSpeed, 0.0f, 200.0f);
    }

    // Movement
    if(ImGui::CollapsingHeader("Movement"))
    {
        ImGui::SliderFloat("Run Speed", &PhysicsSystem::s_RunSpeed, 0.0f, 400.0f);
        ImGui::SliderFloat("Jump Speed", &PhysicsSystem::s_JumpSpeed, 0.0f, 800.0f);
        ImGui::SliderFloat("Terminal Velocity", &PhysicsSystem::s_TerminalVelocity, 0.0f, 1000.0f);
        ImGui::SliderFloat("Air Mobility", &PhysicsSystem::s_AirMobility, 0.0f, 1.0f);
    }

    // Shooting
    if(ImGui::CollapsingHeader("Shooting"))
    {
        ImGui::Text("Active bullets: %d", BulletSystem::s_ActiveBullets);
        ImGui::SliderFloat("Bullet Speed", &BulletSystem::s_BulletSpeed, 0.0f, 800.0f);
        ImGui::SliderFloat("Recoil Speed", &BulletSystem::s_PlayerRecoil, 0.0f, 100.0f);
    }

    if (ImGui::CollapsingHeader("Weapons"))
    {
        Engine::Registry().view<Player>().each([&](Player& player) {
            ImGui::Text("Player with %d weapons", player.weapons.size());
            //for (const auto& weapon : player.weapons) {
              //  ImGui::Text("Weapon!");
            //}
        });
    }

    // Removed these because they are wrong anyway
    //ImGui::Separator();
    //ImGui::Text("Cam bound left: %f", BulletSystem::s_CameraBoundLeft);
    //ImGui::Text("Cam bound right: %f", BulletSystem::s_CameraBoundRight);
    //ImGui::Text("Cam bound up: %f", BulletSystem::s_CameraBoundUp);
    //ImGui::Text("Cam bound down: %f", BulletSystem::s_CameraBoundDown);
     
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
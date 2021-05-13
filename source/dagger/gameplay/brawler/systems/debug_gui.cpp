#include "debug_gui.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include "gameplay/brawler/components/movable.h"
#include "gameplay/brawler/entities/character.h"
#include "gameplay/brawler/systems/physics.h"
#include "gameplay/brawler/systems/bullet_system.h"
#include "gameplay/brawler/systems/hud_system.h"

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
            ImGui::Text("Player %s:", player.side? "right": "left");
            for (int i = 0; i < player.weapons.size(); i++) {
                const auto& weapon = player.weapons[i];
                ImGui::Text("\t%s%s : x%d [%d]", (i == player.active_weapon_idx ? "> " : ""), weapon.name().c_str(), weapon.currentAmmoInClip(), weapon.numClips());
            }
            ImGui::Separator();
        });
    }

    if (ImGui::CollapsingHeader("HUD"))
    {
        ImGui::Text("Cam left: %f", BulletSystem::s_CameraBoundLeft);
        ImGui::Text("Cam right: %f", BulletSystem::s_CameraBoundRight);
        ImGui::Text("Cam up: %f", BulletSystem::s_CameraBoundUp);
        ImGui::Text("Cam down: %f", BulletSystem::s_CameraBoundDown);
        ImGui::SliderFloat("Blip width: ", &HUDSystem::s_blipWidth, 0.0f, 5.0f);
        ImGui::SliderFloat("Blip height: ", &HUDSystem::s_blipHeight, 0.0f, 100.0f);
        ImGui::SliderFloat("Weapon dim: ", &HUDSystem::s_weaponDim, 0, 100);
        ImGui::SliderFloat("Active weapon indicator size: ", &HUDSystem::s_activeWeaponIndicatorSize, 0.0f, 100.0f);
        ImGui::SliderFloat("Padding up: ", &HUDSystem::s_paddingUp, 0.0f, 50.0f);
        ImGui::SliderFloat("Padding side: ", &HUDSystem::s_paddingSide, 0.0f, 50.0f);
        ImGui::SliderFloat("Mid padding up: ", &HUDSystem::s_midPaddingUp, 0.0f, 50.0f);
        ImGui::SliderFloat("Mid padding side: ", &HUDSystem::s_midPaddingSide, 0.0f, 50.0f);
        ImGui::SliderInt("Left player health: ", &HUDSystem::s_leftPlayerHealth, 0, 100);
        ImGui::SliderInt("Right player health: ", &HUDSystem::s_rightPlayerHealth, 0, 100);
        
    }
     
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
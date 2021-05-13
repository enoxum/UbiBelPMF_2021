#include "gameplay/brawler/brawler_main.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/audio.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite_render.h"
#include "core/graphics/textures.h"
#include "core/graphics/animations.h"
#include "core/graphics/gui.h"
#include "core/graphics/text.h"

#include "tools/diagnostics.h"

#include "gameplay/common/simple_collisions.h"
#include "gameplay/brawler/entities/character.h"
#include "gameplay/brawler/systems/bullet_system.h"
#include "gameplay/brawler/systems/character_controller.h"
#include "gameplay/brawler/systems/debug_gui.h"
#include "gameplay/brawler/systems/physics.h"
#include "gameplay/brawler/systems/shooting_system.h"
#include "gameplay/brawler/systems/weaponpickupsystem.h"
#include "gameplay/brawler/systems/hud_system.h"

using namespace dagger;
using namespace brawler;


Entity Brawler::leftPlayer{ entt::null };
Entity Brawler::rightPlayer{ entt::null };

void Brawler::GameplaySystemsSetup()
{
    auto& engine = Engine::Instance();
    engine.AddPausableSystem<CharacterControllerSystem>();
    engine.AddPausableSystem<ShootingSystem>();
    engine.AddPausableSystem<PhysicsSystem>();
    engine.AddPausableSystem<BulletSystem>();
    engine.AddSystem<SimpleCollisionsSystem>();
    engine.AddSystem<WeaponPickupSystem>();
    engine.AddSystem<HUDSystem>();

    engine.AddSystem<DebugGui>();

}

void Brawler::SetCamera()
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 2;
    camera->position = { 0, 100, 0 };
    camera->Update();
}

void Brawler::CreateBackdrop()
{
    auto& reg = Engine::Registry();
    auto* camera = Engine::GetDefaultResource<Camera>();

    /* Create terrain */ {
        auto back = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(back);

        AssignSprite(sprite, "EmptyWhitePixel");
        sprite.color = { 0, 0, 0, 1 };
        sprite.size = { 200, 200 };
        sprite.scale = { 10, 1 };
        sprite.position = { 0, -125, 1 };
    }

    /* Put background image */ {
        auto entity = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);

        AssignSprite(sprite, "souls_like_knight_character:BACKGROUND:Background");
        sprite.position.z = 10;
    }

    ///* Put grass */ {
    //    auto entity = reg.create();
    //    auto& sprite = reg.get_or_emplace<Sprite>(entity);
    //    auto& parallax = reg.get_or_emplace<Parallax>(entity);
    //    parallax.lastCameraPosition = camera->position;
    //    parallax.strength = 0.5f;

    //    AssignSprite(sprite, "souls_like_knight_character:BACKGROUND:Grass");
    //    sprite.position = { 0, -25, 5 };
    //}

    ///* Put trees */ {
    //    auto entity = reg.create();
    //    auto& sprite = reg.get_or_emplace<Sprite>(entity);
    //    auto& parallax = reg.get_or_emplace<Parallax>(entity);
    //    parallax.lastCameraPosition = camera->position;
    //    parallax.strength = 0.25f;

    //    AssignSprite(sprite, "souls_like_knight_character:BACKGROUND:Tree");
    //    sprite.position = { 0, 30, 7 };
    //}

    //{
    //    auto ui = reg.create();
    //    auto& text = reg.emplace<Text>(ui);
    //    text.spacing = 0.6f;
    //    text.Set("pixel-font", "hello world");
    //}
}

void Brawler::WorldSetup()
{
    SetCamera();
    // CreateBackdrop();

    auto c1 = BrawlerCharacter::Create("controller_1", { 1, 1, 1 }, { 0, 0 }, false);
    Brawler::leftPlayer = c1.entity;
    HUDSystem::CreateHealthBarLeft();
    HUDSystem::CreateWeaponsLeft();

    auto c2 = BrawlerCharacter::Create("Arrows", { 1, 1, 1 }, { 0, 0 }, true);
    Brawler::rightPlayer = c2.entity;
    HUDSystem::CreateHealthBarRight();
    HUDSystem::CreateWeaponsRight();


    HUDSystem::CreateMarkersAndTexts();
    
    

    
    
    
    
    //Engine::Registry().emplace<CameraFollowFocus>(player1.entity);

    // auto player2 = BrawlerCharacter::Create("Arrows", { 1, 0, 0 }, { 100, 0 });
    //Engine::Registry().emplace<CameraFollowFocus>(player2.entity);

    // auto nzm = Engine::Registry().create();
    // Engine::Registry().get_or_emplace<Transform>(nzm);
    // auto& x = Engine::Registry().get_or_emplace<Sprite>(nzm);
    // x.scale = { 1, 1 };
    // x.position = { 0.0f, 0.0f, 0.0f };
    // x.color = { 1.0f, 1.0f, 1.0f, 1.0f };
    // AssignSprite(x, "spritesheets:2lisp:Gunner_Green_Idle:idle:1");

}
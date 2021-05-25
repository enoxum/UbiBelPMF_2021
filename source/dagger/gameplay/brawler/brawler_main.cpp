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
#include "gameplay/brawler/debug_tools/camera_control_system.h"
#include "gameplay/brawler/debug_tools/debug_gui.h"
#include "gameplay/brawler/entities/character.h"
#include "gameplay/brawler/systems/bullet_system.h"
#include "gameplay/brawler/systems/character_controller.h"
#include "gameplay/brawler/systems/physics.h"
#include "gameplay/brawler/systems/shooting_system.h"
#include "gameplay/brawler/systems/level_system.h"
#include "gameplay/brawler/systems/map_collision_system.h"
#include "gameplay/brawler/level.h"
#include "gameplay/brawler/systems/weapon_spawn_system.h"
#include "gameplay/brawler/systems/weaponpickupsystem.h"
#include "gameplay/brawler/systems/drop_system.h"
#include "gameplay/brawler/systems/hud_system.h"
#include "gameplay/brawler/systems/bullet_collision_system.h"
#include "gameplay/brawler/systems/projectile_collision_system.h"

using namespace dagger;
using namespace brawler;

void Brawler::GameplaySystemsSetup()
{
    auto& engine = Engine::Instance();
    engine.AddSystem<LevelSystem>();
    
    engine.AddPausableSystem<PhysicsSystem>();
    engine.AddPausableSystem<CharacterControllerSystem>();
    engine.AddPausableSystem<MapCollisionSystem>();

    engine.AddPausableSystem<ShootingSystem>();
    engine.AddPausableSystem<BulletSystem>();
    engine.AddPausableSystem<WeaponSpawnSystem>();
    engine.AddPausableSystem<WeaponPickupSystem>();
    engine.AddPausableSystem<DropSystem>();
    engine.AddPausableSystem<HUDSystem>();
    engine.AddPausableSystem<BulletCollisionSystem>();
    engine.AddPausableSystem<ProjectileCollisionSystem>();
    
    // Debug Systems
    engine.AddSystem<CameraControlSystem>();
    engine.AddSystem<DebugGui>();
}

void Brawler::SetCamera()
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 1280, 720 };
    camera->zoom = 2;
    camera->position = { 320, 180, 0 };
    camera->Update();
}

void Brawler::WorldSetup()
{
    SetCamera();
    Level::Load("default");
}

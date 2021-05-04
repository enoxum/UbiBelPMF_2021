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
#include "gameplay/brawler/level.h"

using namespace dagger;
using namespace brawler;

void Brawler::GameplaySystemsSetup()
{
    auto& engine = Engine::Instance();
    engine.AddPausableSystem<CharacterControllerSystem>();
    engine.AddPausableSystem<ShootingSystem>();
    engine.AddPausableSystem<PhysicsSystem>();
    engine.AddPausableSystem<BulletSystem>();
    engine.AddSystem<SimpleCollisionsSystem>();
    
    // Debug Systems
    engine.AddSystem<CameraControlSystem>();
    engine.AddSystem<DebugGui>();
}

void Brawler::SetCamera()
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 2;
    camera->position = { 200, 150, 0 };
    camera->Update();
}

void Brawler::WorldSetup()
{
    SetCamera();
    Level::Create();

    auto player1 = BrawlerCharacter::Create("controller_1", { 1, 1, 1 }, { 200, 32 });
    //Engine::Registry().emplace<CameraFollowFocus>(player1.entity);

    // auto player2 = BrawlerCharacter::Create("Arrows", { 1, 0, 0 }, { 100, 0 });
    //Engine::Registry().emplace<CameraFollowFocus>(player2.entity);
}

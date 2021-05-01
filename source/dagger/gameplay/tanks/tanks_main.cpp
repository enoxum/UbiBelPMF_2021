#include "core/core.h"
#include "core/engine.h"
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
#include "tools/diagnostics.h"

#include "gameplay/common/simple_collisions.h"
#include "gameplay/tanks/tanks_main.h"
#include "tilemap.h"

using namespace dagger;
using namespace tanks;


void TanksGame::CoreSystemsSetup(Engine& engine_)
{
    engine_.AddSystem<WindowSystem>();
    engine_.AddSystem<InputSystem>();
    engine_.AddSystem<ShaderSystem>();
    engine_.AddSystem<TextureSystem>();
    engine_.AddSystem<SpriteRenderSystem>();
    engine_.AddPausableSystem<TransformSystem>();
    engine_.AddPausableSystem<AnimationSystem>();
#if !defined(NDEBUG)
    engine_.AddSystem<DiagnosticSystem>();
    engine_.AddSystem<GUISystem>();
    engine_.AddSystem<ToolMenuSystem>();
   
#endif //!defined(NDEBUG)
}

void TanksGame::GameplaySystemsSetup(Engine& engine_)
{

    engine_.AddPausableSystem<SimpleCollisionsSystem>();
    engine_.AddSystem<TilemapSystem>();
    
#if defined(DAGGER_DEBUG)
    
#endif //defined(DAGGER_DEBUG)
}

void TanksGame::WorldSetup(Engine& engine_)
{
   
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 900, 900 };
    camera->zoom = 1;
    camera->position = { 420, 410, 0 };
    camera->Update();

    SetupWorld(engine_);
}
#define BLOCK_SIZE 42
Entity CreateFloor(Registry& reg_, UInt32 x_, UInt32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * BLOCK_SIZE, y_ * BLOCK_SIZE, 90 };
   
    AssignSprite(sprite, "blk3");
    sprite.size = { BLOCK_SIZE, BLOCK_SIZE };
    return entity;
}

Entity CreateOuterWall(Registry& reg_, UInt32 x_, UInt32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * BLOCK_SIZE, y_ * BLOCK_SIZE, 90 };
    
    AssignSprite(sprite, "blk1");
    sprite.size = { BLOCK_SIZE, BLOCK_SIZE };
    return entity;

}
Entity CreateInnerWall(Registry& reg_, UInt32 x_, UInt32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * BLOCK_SIZE, y_ * BLOCK_SIZE, 90 };

    AssignSprite(sprite, "blk5");
    sprite.size = { BLOCK_SIZE, BLOCK_SIZE };
    return entity;

}
Entity CreateWallFlower(Registry& reg_, UInt32 x_, UInt32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * BLOCK_SIZE, y_ * BLOCK_SIZE, 90 };

    AssignSprite(sprite, "blk4");
    sprite.size = { BLOCK_SIZE, BLOCK_SIZE };
    return entity;

}
void tanks::SetupWorld(Engine& engine_)
{
    TilemapLegend legend;
    legend['#'] = &CreateOuterWall;
    legend['.'] = &CreateFloor;
    legend['*'] = &CreateWallFlower;
    legend['$'] = &CreateInnerWall;
    Engine::Dispatcher().trigger<TilemapLoadRequest>(TilemapLoadRequest{ "my-file.map.txt", &legend });

}
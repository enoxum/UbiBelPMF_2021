#include "hotline_miami_main.h"
#include "hotline_miami_obstacle.h"
#include "hotline_miami_player.h"
#include "gameplay/hotline_miami/hotline_miami_projectile.h"
#include "gameplay/hotline_miami/hotline_miami_tools.h"
#include "gameplay/hotline_miami/hotline_miami_player_input.h"
#include "gameplay/hotline_miami/hotline_miami_camera_focus.h"
#include "gameplay/hotline_miami/hotline_miami_weapon.h"
#include "gameplay/hotline_miami/hotline_miami_enemy.h"
#include "gameplay/hotline_miami/hotline_miami_health.h"
#include "gameplay/hotline_miami/hotline_miami_level.h"

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





using namespace dagger;
using namespace hotline_miami;


void HotlineMiamiGame::CoreSystemsSetup()
{
    auto& engine = Engine::Instance();

    engine.AddSystem<WindowSystem>();
    engine.AddSystem<InputSystem>();
    engine.AddSystem<ShaderSystem>();
    engine.AddSystem<TextureSystem>();
    engine.AddSystem<SpriteRenderSystem>();
    engine.AddPausableSystem<TransformSystem>();
    engine.AddPausableSystem<AnimationSystem>();
#if !defined(NDEBUG)
    engine.AddSystem<DiagnosticSystem>();
    engine.AddSystem<GUISystem>();
    engine.AddSystem<ToolMenuSystem>();
#endif //!defined(NDEBUG)
}

void HotlineMiamiGame::GameplaySystemsSetup()
{
    auto& engine = Engine::Instance();

    engine.AddPausableSystem<SimpleCollisionsSystem>();
    engine.AddPausableSystem<HotlineMiamiCameraFollowSystem>();
    engine.AddPausableSystem<HotlineMiamiPlayerInputSystem>();
    engine.AddPausableSystem<HotlineMiamiWorldEndSystem>();
    engine.AddPausableSystem<HotlineMiamiObstacleSystem>();
    engine.AddPausableSystem<HotlineMiamiPlayerObstacleCollisionSystem>();
    engine.AddPausableSystem<HotlineMiamiWeaponSystem>();
    engine.AddPausableSystem<HotlineMiamiKeySystem>();
    engine.AddPausableSystem<HotlineMiamiHealthSystem>();
    engine.AddPausableSystem<HotlineMiamiProjectileSystem>();
    engine.AddPausableSystem<HotlineMiamiProjectileObstacleCollisionSystem>();
    engine.AddPausableSystem<HotlineMiamiEnemyProjectileCollisionSystem>();
    engine.AddPausableSystem<HotlineMiamiEnemyBulletSystem>();
    engine.AddPausableSystem<HotlineMiamiEnemyBulletPlayerCollisionSystem>();
    engine.AddPausableSystem<HotlineMiamiEnemyBulletObstacleCollisionSystem>();
    engine.AddPausableSystem<HotlineMiamiDeleteEntitySystem>();
}

void HotlineMiamiGame::WorldSetup()
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 1;
    camera->position = { 0, 0, 0 };
    camera->Update();

    SetupWorld();
}

void hotline_miami::SetupWorld()
{
    Vector2 scale(1, 1);

    auto& engine = Engine::Instance();
    auto& reg = engine.Registry();

    // field
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 600;

    constexpr int height = 32;
    constexpr int width = 32;
    constexpr float tileSize = 80.f;// / static_cast<float>(Width);
    constexpr float playerSize = 40.f;

    float zPos = 5.f;

    loadTiles(zPos);

    //loadWorldEdges(zPos);

    zPos -= 1.f;

    loadObstacles(zPos);

    loadEnemies(zPos, playerSize);

    // world end
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = tileSize* 2;
        col.size.y = tileSize * 10;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = 700;
        transform.position.y = -350;
        transform.position.z = zPos;

        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "hotline_miami:Obstacle:obstacle_world_end");
        sprite.size.x = tileSize * 2;
        sprite.size.y = tileSize * 10;

        auto& key = reg.emplace<HotlineMiamiWorldEnd>(entity);
        auto& obstacle = reg.emplace<HotlineMiamiObstacle>(entity);
    }

    // health
    {
        auto entity = reg.create();
        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = 800;
        transform.position.y = 50;
        transform.position.z = zPos - 2.f;

        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "hotline_miami:Health:health_3");
        sprite.size.x = 160.f;
        sprite.size.y = 32.f;

        auto& key = reg.emplace<HotlineMiamiHealth>(entity);

    }

    // key
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = playerSize;
        col.size.y = playerSize * 0.5;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = -800;
        transform.position.y = 50;
        transform.position.z = zPos;

        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "hotline_miami:Obstacle:obstacle_key");
        sprite.size.x = playerSize;
        sprite.size.y = playerSize * 0.5;

        auto& key = reg.emplace<HotlineMiamiKey>(entity);
    }

    // bazuka
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = playerSize * 0.6;
        col.size.y = playerSize * 1.5;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = 300;
        transform.position.y = 50;
        transform.position.z = zPos;

        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "hotline_miami:Weapon:weapon_bazuka");
        sprite.size.x = playerSize * 0.6;
        sprite.size.y = playerSize * 1.5;

        auto& weapon = reg.emplace<HotlineMiamiWeapon>(entity);
        weapon.type = 2;
    }

    // pistol
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = playerSize * 0.3;
        col.size.y = playerSize * 0.6;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = 200;
        transform.position.y = 50;
        transform.position.z = zPos;

        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "hotline_miami:Weapon:weapon_pistol");
        sprite.size.x = playerSize * 0.3;
        sprite.size.y = playerSize * 0.6;

        auto& weapon = reg.emplace<HotlineMiamiWeapon>(entity);
        weapon.type = 1;
    }

    // player
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = playerSize;
        col.size.y = playerSize;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = 100;
        transform.position.y = 100;
        transform.position.z = zPos;

        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "hotline_miami:Player:player_unarmed");
        sprite.size.x = playerSize;
        sprite.size.y = playerSize;

        auto& player = reg.emplace<HotlineMiamiPlayer>(entity);

        auto& controller = reg.emplace<ControllerMapping>(entity);

        auto& camera = reg.emplace<HotlineMiamiCameraFollowFocus>(entity);

        HotlineMiamiPlayerInputSystem::SetupPlayerInput(controller);
    }
}

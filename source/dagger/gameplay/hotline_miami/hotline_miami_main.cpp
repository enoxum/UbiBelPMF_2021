#include "hotline_miami_main.h"
#include "hotline_miami_obstacle.h"
#include "hotline_miami_player.h"
#include "gameplay/hotline_miami/hotline_miami_projectile.h"
#include "gameplay/hotline_miami/hotline_miami_tools.h"
#include "gameplay/hotline_miami/hotline_miami_player_input.h"
#include "gameplay/hotline_miami/hotline_miami_camera_focus.h"
#include "gameplay/hotline_miami/hotline_miami_weapon.h"
#include "gameplay/hotline_miami/hotline_miami_enemy.h"

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
    engine.AddPausableSystem<HotlineMiamiObstacleSystem>();
    engine.AddPausableSystem<HotlineMiamiPlayerObstacleCollisionSystem>();
    engine.AddPausableSystem<HotlineMiamiWeaponSystem>();
    engine.AddPausableSystem<HotlineMiamiKeySystem>();
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

    float zPos = 1.f;

    constexpr float Space = -0.1f;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);

            if (j >= 22 && j <= 24) 
            {
                AssignSprite(sprite, "hotline_miami:Terrain:terrain_pavement");
            }
            else
            {
                AssignSprite(sprite, "hotline_miami:Terrain:terrain_grass");
            }

            sprite.size = scale * tileSize;

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = (j + j * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
            transform.position.y = (i + i * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;
            transform.position.z = zPos;
        }
    }

    zPos -= 1.f;

    // key
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = playerSize;
        col.size.y = playerSize * 0.5;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = 700;
        transform.position.y = 50;
        transform.position.z = zPos;

        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "hotline_miami:Obstacle:obstacle_key");
        sprite.size.x = playerSize;
        sprite.size.y = playerSize * 0.5;

        auto& key = reg.emplace<HotlineMiamiKey>(entity);
    }

    // enemy 1
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = playerSize;
        col.size.y = playerSize;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = 500;
        transform.position.y = 50;
        transform.position.z = zPos;

        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "hotline_miami:Enemy:enemy_pistol");
        sprite.size.x = playerSize;
        sprite.size.y = playerSize;

        auto& enemey = reg.emplace<HotlineMiamiEnemy>(entity);
    }
    // enemy 2 
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = playerSize;
        col.size.y = playerSize;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = -500;
        transform.position.y = 50;
        transform.position.z = zPos;

        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "hotline_miami:Enemy:enemy_pistol");
        sprite.size.x = playerSize;
        sprite.size.y = playerSize;

        auto& enemey = reg.emplace<HotlineMiamiEnemy>(entity);
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

    // obstacle house
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = tileSize * 4;
        col.size.y = tileSize * 2;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = 200;
        transform.position.y = -200;
        transform.position.z = zPos;

        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "hotline_miami:Obstacle:obstacle_house_1");
        sprite.size.x = tileSize * 4;
        sprite.size.y = tileSize * 2;

        auto& obstacle = reg.emplace<HotlineMiamiObstacle>(entity);
    }

    // obstacle house
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = tileSize * 2;
        col.size.y = tileSize * 2;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = -150;
        transform.position.y = 0;
        transform.position.z = zPos;

        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "hotline_miami:Obstacle:obstacle_house_2");
        sprite.size.x = tileSize * 2;
        sprite.size.y = tileSize * 2;

        auto& obstacle = reg.emplace<HotlineMiamiObstacle>(entity);
    }

    // obstacle house
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = tileSize * 4;
        col.size.y = tileSize * 2;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = 100;
        transform.position.y = 200;
        transform.position.z = zPos;

        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "hotline_miami:Obstacle:obstacle_house_1");
        sprite.size.x = tileSize * 4;
        sprite.size.y = tileSize * 2;

        auto& obstacle = reg.emplace<HotlineMiamiObstacle>(entity);
    }

    // obstacle tree
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = tileSize * 2;
        col.size.y = tileSize * 2;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = -400;
        transform.position.y = -400;
        transform.position.z = zPos;

        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "hotline_miami:Obstacle:obstacle_tree_1");
        sprite.size.x = tileSize * 2;
        sprite.size.y = tileSize * 2;

        auto& obstacle = reg.emplace<HotlineMiamiObstacle>(entity);
    }

    // obstacle tree
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = tileSize * 2;
        col.size.y = tileSize * 2;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = -550;
        transform.position.y = -550;
        transform.position.z = zPos;

        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "hotline_miami:Obstacle:obstacle_tree_2");
        sprite.size.x = tileSize * 2;
        sprite.size.y = tileSize * 2;

        auto& obstacle = reg.emplace<HotlineMiamiObstacle>(entity);
    }

    // obstacle tree
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = tileSize * 2;
        col.size.y = tileSize * 2;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = 800;
        transform.position.y = 500;
        transform.position.z = zPos;

        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "hotline_miami:Obstacle:obstacle_tree_3");
        sprite.size.x = tileSize * 2;
        sprite.size.y = tileSize * 2;

        auto& obstacle = reg.emplace<HotlineMiamiObstacle>(entity);
    }

    // obstacle rock
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = tileSize;
        col.size.y = tileSize;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = -300;
        transform.position.y = 300;
        transform.position.z = zPos;

        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "hotline_miami:Obstacle:obstacle_rock_2");
        sprite.size.x = tileSize;
        sprite.size.y = tileSize;

        auto& obstacle = reg.emplace<HotlineMiamiObstacle>(entity);
    }

    // obstacle rock
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = tileSize * 2;
        col.size.y = tileSize * 2;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = -400;
        transform.position.y = 400;
        transform.position.z = zPos;

        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "hotline_miami:Obstacle:obstacle_rock_3");
        sprite.size.x = tileSize * 2;
        sprite.size.y = tileSize * 2;

        auto& obstacle = reg.emplace<HotlineMiamiObstacle>(entity);
    }

}

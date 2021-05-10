#include "pandemic_shop_main.h"

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
#include "core/game/transforms.h"

#include "gameplay/common/simple_collisions.h"
#include "gameplay/ping_pong/pingpong_ball.h"
#include "gameplay/ping_pong/player_scores.h"
#include "gameplay/ping_pong/pingpong_tools.h"
#include "gameplay/PandemicShop/pandemic_player_input.h"
#include "gameplay/PandemicShop/pandemic_tools.h"
#include "gameplay/PandemicShop/pandemic_character_controller.h";


using namespace dagger;
using namespace pandemic_shop;
//---------------------------
using namespace pandemic;


void PandemicShopGame::CoreSystemsSetup(Engine& engine_)
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
    engine_.AddSystem<CollisionDetectionSystem>();
    engine_.AddSystem<GUISystem>();
    engine_.AddSystem<ToolMenuSystem>();
#endif //!defined(NDEBUG)
}

void PandemicShopGame::GameplaySystemsSetup(Engine& engine_)
{
    engine_.AddPausableSystem<SimpleCollisionsSystem>();
    engine_.AddPausableSystem<PandemicControllerSystem>();
#if defined(DAGGER_DEBUG)
    engine_.AddPausableSystem<ping_pong::PingPongTools>();
#endif //defined(DAGGER_DEBUG)
}

void PandemicShopGame::WorldSetup(Engine& engine_)
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 1;
    camera->position = { 0, 0, 0 };
    camera->Update();

    SetupWorld(engine_);
}
//---------------------------------------------------------
struct Character {
  Entity entity;
  Sprite &sprite;
  Animator &animator;
  InputReceiver &input;
  PandemicCharacter &character;
  Transform &transform;

  static Character Get(Entity entity) {
    auto &reg = Engine::Registry();
    auto &sprite = reg.get_or_emplace<Sprite>(entity);
    auto &anim = reg.get_or_emplace<Animator>(entity);
    auto &input = reg.get_or_emplace<InputReceiver>(entity);
    auto &character = reg.get_or_emplace<PandemicCharacter>(entity);
    auto &transform = reg.get_or_emplace<Transform>(entity);

    
    //return Character{entity, sprite, anim, input};
    return Character{entity, sprite, anim, input, character, transform};
  }

  static Character Create(String input_ = "", ColorRGB color_ = {1, 1, 1},
                          Vector2 position_ = {0, 0}) {
    auto &reg = Engine::Registry();
    auto entity = reg.create();

    ATTACH_TO_FSM(PandemicCharacterControllerFSM, entity);

    auto chr = Character::Get(entity);

    chr.transform.position = {position_, 0.0f};

    chr.sprite.scale = {2, 2};
    chr.sprite.position = {position_, 0.0f};
    chr.sprite.color = {color_, 1.0f};

    AssignSprite(chr.sprite, "PandemicShop:BOB_IDLE:FRONT:bob_idle1");
    AnimatorPlay(chr.animator, "PandemicShop:IDLE_FRONT");

    if (input_ != "")
      chr.input.contexts.push_back(input_);

    chr.character.speed = 50;

    return chr;
  }
};
//--------------------------------------------

void pandemic_shop::SetupWorld(Engine& engine_)
{
    Vector2 scale(1, 1);

    auto& reg = engine_.Registry();

    // field
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 600;

    constexpr int height = 20;
    constexpr int width = 26;
    constexpr float tileSize = 20.f;// / static_cast<float>(Width);

    float zPos = 1.f;

    constexpr float Space = 0.1f;

    for (int i = 0; i < height; i++)
    {
        
        for (int j = 0; j < width; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);
            AssignSprite(sprite, "EmptyWhitePixel");
            sprite.size = scale * tileSize;

            if (i % 2 != j % 2)
            {
                sprite.color.r = 0.4f;
                sprite.color.g = 0.4f;
                sprite.color.b = 0.4f;
            }
            else
            {
                sprite.color.r = 0.6f;
                sprite.color.g = 0.6f;
                sprite.color.b = 0.6f;
                
            }

            if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
            {
                sprite.color.r = 0.0f;
                sprite.color.g = 0.0f;
                sprite.color.b = 0.0f;

                auto &col = reg.emplace<SimpleCollision>(entity);
                col.size.x = tileSize;
                col.size.y = tileSize;
                
            }

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = (0.5f + j + j * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
            transform.position.y = (0.5f + i + i * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;
            transform.position.z = zPos;
        }
    }

    zPos -= 1.f;

    // collisions
    {
        // up
        {
            auto entity = reg.create();
            auto& col = reg.emplace<SimpleCollision>(entity);
            col.size.x = tileSize * (width - 2)* (1 + Space);
            col.size.y = tileSize;

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = 0;
            transform.position.y = (0.5f + (height - 1) + (height - 1) * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;
            transform.position.z = zPos;
        }

        // down
        {
            auto entity = reg.create();
            auto& col = reg.emplace<SimpleCollision>(entity);
            col.size.x = tileSize * (width - 2) * (1 + Space);
            col.size.y = tileSize;

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = 0;
            transform.position.y = (0.5f - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;
            transform.position.z = zPos;
        }

        // left
        {
            auto entity = reg.create();
            auto& col = reg.emplace<SimpleCollision>(entity);
            col.size.x = tileSize;
            col.size.y = tileSize * (height - 2) * (1 + Space);

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = (0.5f - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
            transform.position.y = 0;
            transform.position.z = zPos;
        }

        // right
        {
            auto entity = reg.create();
            auto& col = reg.emplace<SimpleCollision>(entity);
            col.size.x = tileSize;
            col.size.y = tileSize * (height - 2) * (1 + Space);

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = (0.5f + (width - 1) + (width - 1) * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
            transform.position.y = 0;
            transform.position.z = zPos;

        }
    }

    const Float32 playerSize = tileSize;

    // player controller setup
    {
        auto* stats = new pandemic_shop::GameStats;
        stats->SetupPlayerBorders((height - 2)* (tileSize + Space + 5) / 2.f,
            -(height - 2) * (tileSize + Space + 5) / 2.f,
            (width - 2)* (tileSize + Space + 5) / 2.f,
            -(width - 2) * (tileSize + Space + 5) / 2.f);
        stats->PlayerSpeed = tileSize * 14.f;
        Engine::Instance().PutDefaultResource<pandemic_shop::GameStats>(stats);
    }

    //1st player
    {
     /*   auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = playerSize;
        col.size.y = playerSize;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = (2.5f - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;
        transform.position.y = 0;
        transform.position.z = zPos;

         // player 
        
        auto &sprite = reg.emplace<Sprite>(entity);
       AssignSprite(sprite, "PandemicShop:BOB_IDLE:FRONT:bob_idle1");
        float ratio = sprite.size.y / sprite.size.x;
        sprite.size = {2 * tileSize, 2 * tileSize * ratio};
        */
        Character::Create("Pandemic", {1, 1, 1}, {0, 0});
    }

    // // add score system to count scores for left and right collisions
    // PlayerScoresSystem::SetFieldSize(width, height, tileSize * (1 + Space));
}

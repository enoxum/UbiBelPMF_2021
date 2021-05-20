#include "pandemic_shop_main.h"
#include "core/game/transforms.h"
#include "core/graphics/animation.h"
#include "core/graphics/animations.h"
#include "core/graphics/gui.h"
#include "core/graphics/shaders.h"
#include "core/graphics/sprite.h"
#include "core/graphics/sprite_render.h"
#include "core/graphics/textures.h"
#include "core/graphics/window.h"
#include "core/input/inputs.h"
#include "tools/diagnostics.h"

#include "gameplay/PandemicShop/game_menu.h"
#include "gameplay/PandemicShop/item.h"
#include "gameplay/PandemicShop/pandemic_character_controller.h"
#include "gameplay/PandemicShop/pandemic_player_input.h"
#include "gameplay/PandemicShop/pandemic_tools.h"
#include "gameplay/PandemicShop/player.h"
#include "gameplay/common/simple_collisions.h"
#include "gameplay/ping_pong/pingpong_ball.h"
#include "gameplay/ping_pong/pingpong_tools.h"
#include "gameplay/ping_pong/player_scores.h"
#include <core/graphics/text.h>

using namespace dagger;
using namespace pandemic_shop;
//---------------------------
using namespace pandemic;

void PandemicShopGame::CoreSystemsSetup(Engine &engine_) {
  engine_.AddSystem<WindowSystem>();
  engine_.AddSystem<InputSystem>();
  engine_.AddSystem<ShaderSystem>();
  engine_.AddSystem<TextureSystem>();
  engine_.AddSystem<SpriteRenderSystem>();
  engine_.AddPausableSystem<TransformSystem>();
  engine_.AddPausableSystem<AnimationSystem>();
#if !defined(NDEBUG)
  engine_.AddSystem<DiagnosticSystem>();
  // engine_.AddSystem<CollisionDetectionSystem>();
  engine_.AddSystem<GUISystem>();
  engine_.AddSystem<ToolMenuSystem>();
#endif //! defined(NDEBUG)
}

void PandemicShopGame::GameplaySystemsSetup(Engine &engine_) {
  engine_.AddPausableSystem<SimpleCollisionsSystem>();
  engine_.AddPausableSystem<PandemicControllerSystem>();
  engine_.AddPausableSystem<CollisionDetectionSystem>();
  engine_.AddPausableSystem<Pickable>();
#if defined(DAGGER_DEBUG)
  engine_.AddPausableSystem<ping_pong::PingPongTools>();
  engine_.AddPausableSystem<GameMenuSystem>();
#endif // defined(DAGGER_DEBUG)
}

void PandemicShopGame::WorldSetup(Engine &engine_) {
  auto *camera = Engine::GetDefaultResource<Camera>();
  camera->mode = ECameraMode::FixedResolution;
  camera->size = {800, 600};
  camera->zoom = 1;
  camera->position = {0, 0, 0};
  camera->Update();
  SetupWorld(engine_);
}
//---------------------------------------------------------

void pandemic_shop::SetupWorld(Engine &engine_) {

  pandemic_shop::SetupStartScreen(engine_);
  Vector2 scale(1, 1);

  auto &reg = engine_.Registry();

  // field
  constexpr int screenWidth = 800;
  constexpr int screenHeight = 600;

  constexpr int height = 20;
  constexpr int width = 26;
  constexpr float tileSize = 20.f; // / static_cast<float>(Width);

  float zPos = 1.f;

  constexpr float Space = 0.1f;

  for (int i = 0; i < height; i++) {

    for (int j = 0; j < width; j++) {
      auto entity = reg.create();
      auto &sprite = reg.emplace<Sprite>(entity);
      AssignSprite(sprite, "EmptyWhitePixel");
      sprite.size = scale * tileSize;

      if (i % 2 != j % 2) {
        sprite.color.r = 0.4f;
        sprite.color.g = 0.4f;
        sprite.color.b = 0.4f;
      } else {
        sprite.color.r = 0.6f;
        sprite.color.g = 0.6f;
        sprite.color.b = 0.6f;
      }

      if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
        sprite.color.r = 0.0f;
        sprite.color.g = 0.0f;
        sprite.color.b = 0.0f;

        auto &col = reg.emplace<SimpleCollision>(entity);
        col.size.x = tileSize;
        col.size.y = tileSize;
      }

      auto &transform = reg.emplace<Transform>(entity);
      transform.position.x = (0.5f + j + j * Space -
                              static_cast<float>(width * (1 + Space)) / 2.f) *
                             tileSize;
      transform.position.y = (0.5f + i + i * Space -
                              static_cast<float>(height * (1 + Space)) / 2.f) *
                             tileSize;
      transform.position.z = zPos;
    }
  }

  zPos -= 1.f;

  // collisions
  {
    // up
    {
      auto entity = reg.create();
      auto &col = reg.emplace<SimpleCollision>(entity);
      col.size.x = tileSize * (width - 2) * (1 + Space);
      col.size.y = tileSize;

      auto &transform = reg.emplace<Transform>(entity);
      transform.position.x = 0;
      transform.position.y = (0.5f + (height - 1) + (height - 1) * Space -
                              static_cast<float>(height * (1 + Space)) / 2.f) *
                             tileSize;
      transform.position.z = zPos;
    }

    // down
    {
      auto entity = reg.create();
      auto &col = reg.emplace<SimpleCollision>(entity);
      col.size.x = tileSize * (width - 2) * (1 + Space);
      col.size.y = tileSize;

      auto &transform = reg.emplace<Transform>(entity);
      transform.position.x = 0;
      transform.position.y =
          (0.5f - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;
      transform.position.z = zPos;
    }

    // left
    {
      auto entity = reg.create();
      auto &col = reg.emplace<SimpleCollision>(entity);
      col.size.x = tileSize;
      col.size.y = tileSize * (height - 2) * (1 + Space);

      auto &transform = reg.emplace<Transform>(entity);
      transform.position.x =
          (0.5f - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
      transform.position.y = 0;
      transform.position.z = zPos;
    }

    // right
    {
      auto entity = reg.create();
      auto &col = reg.emplace<SimpleCollision>(entity);
      col.size.x = tileSize;
      col.size.y = tileSize * (height - 2) * (1 + Space);

      auto &transform = reg.emplace<Transform>(entity);
      transform.position.x = (0.5f + (width - 1) + (width - 1) * Space -
                              static_cast<float>(width * (1 + Space)) / 2.f) *
                             tileSize;
      transform.position.y = 0;
      transform.position.z = zPos;
    }
  }

  const Float32 playerSize = tileSize;

  // player controller setup
  {
    auto *stats = new pandemic_shop::GameStats;
    stats->SetupPlayerBorders((height - 2) * (tileSize + Space + 5) / 2.f,
                              -(height - 2) * (tileSize + Space + 5) / 2.f,
                              (width - 2) * (tileSize + Space + 5) / 2.f,
                              -(width - 2) * (tileSize + Space + 5) / 2.f);
    stats->PlayerSpeed = tileSize * 14.f;
    Engine::Instance().PutDefaultResource<pandemic_shop::GameStats>(stats);
  }

  // 1st player
  {
    Character::Create("Pandemic", {1, 1, 1}, {0, 0});

    /*auto ent = reg.create();
    auto item = reg.emplace<Item>(ent);
    item.Create(ent, "spritesheets:pixel_mart:tuna_can", {1, 1, 1}, {32, 32});
    if(item.hidden){
        printf("\nhidden\n");
    }
    else{

        printf("\n not hidden \n");
    }*/
  }
}

//-------------------------------------------pocetni ekran i restartekran-------------------------------------
void pandemic_shop::SetupStartScreen(Engine &engine_) {
  auto &reg = engine_.Registry();

  auto entity = reg.create();
  auto &sprite = reg.emplace<Sprite>(entity);
  sprite.position.z = 0;

  auto &gmb = reg.emplace<GameMenuButton>(entity);
  auto &input = reg.emplace<InputReceiver>(entity);
  input.contexts.push_back("menu");
  gmb.position = sprite.position;

  AssignSprite(sprite, "PandemicShop:start_button");
  gmb.size = sprite.size;
}

void pandemic_shop::SetupRestartScreen(Engine &engine_, String winner_) {
    auto &reg = engine_.Registry();

    auto entity = reg.create();
    auto &sprite = reg.emplace<Sprite>(entity);
    sprite.position.z = 0;

    auto &gmb = reg.emplace<GameMenuButton>(entity);
    auto &input = reg.emplace<InputReceiver>(entity);
    input.contexts.push_back("menu");
    gmb.position = sprite.position;

    AssignSprite(sprite, "PandemicShop:replay_button");
    gmb.size = sprite.size;
}

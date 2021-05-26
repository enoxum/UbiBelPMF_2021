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
#include "gameplay/PandemicShop/pandemic_character_controller.h"
#include "gameplay/PandemicShop/karen_controller.h"
#include "gameplay/PandemicShop/item.h"
#include "gameplay/PandemicShop/player.h"
#include "gameplay/PandemicShop/karen.h"
#include "gameplay/PandemicShop/ai_system.h"
#include "gameplay/common/simple_collisions.h"
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

void PandemicShopGame::GameplaySystemsSetup(Engine& engine_)
{
    engine_.AddPausableSystem<SimpleCollisionsSystem>();
    engine_.AddPausableSystem<PandemicControllerSystem>();
    engine_.AddPausableSystem<KarenControllerSystem>();
    engine_.AddPausableSystem<CollisionDetectionSystem>();
    engine_.AddPausableSystem<Pickable>();
    engine_.AddPausableSystem<AISystem>();
#if defined(DAGGER_DEBUG)
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
  pandemic_shop::SetupStartScreen(engine_);
}
//---------------------------------------------------------

void pandemic_shop::SetupWorld(Engine &engine_) {
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
        reg.emplace<CollisionType::Wall>(entity);
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
  {
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
    Character::Create("Pandemic", {1, 1, 1}, {64, 64});
        KarenCharacter::Create("Pandemic", {1, 1, 1}, {0, 0});
        KarenCharacter::Create("Pandemic", {0.5, 0.5, 0.5}, {128, 128});
        KarenCharacter::Create("Pandemic", {0.7, 0.7, 0.7}, {-128, 128});

        auto ent1 = reg.create();
        auto item1 = reg.emplace<Item>(ent1);
        
        item1.Create(ent1, "spritesheets:pixel_mart:green_apple", {1, 1, 1}, {164, 164});
        auto& collider = reg.get<SimpleCollision>(ent1);
        collider.size = {32, 32};

        // auto ent2 = reg.create();
        // auto item2 = reg.emplace<Item>(ent2);
        // item2.Create(ent2, "spritesheets:pixel_mart:tuna_can", {1, 1, 1}, {128, -128});

        // auto ent3 = reg.create();
        // auto item3 = reg.emplace<Item>(ent3);
        // item3.Create(ent3, "spritesheets:pixel_mart:tuna_can", {1, 1, 1}, {-128, 128});

        auto ent4 = reg.create();
        auto item4 = reg.emplace<Item>(ent4);
        item4.Create(ent4, "spritesheets:pixel_mart:tuna_can", {1, 1, 1}, {-128, -128});

        // if(item.hidden){
        //     printf("\nhidden\n");
        // }
        // else{

        //     printf("\n not hidden \n");
        // }

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

  //text
  auto entityt = reg.create();
  auto &text = reg.emplace<Text>(entityt);
  text.spacing = 0.6f;
  text.letterSize = {37.0f, 47.0f};
  text.Set("pixel-font","Pandemic Shop Game", {10, 175, 98});

  auto entityt2 = reg.create();
  auto &text2 = reg.emplace<Text>(entityt2);
  text2.letterSize = {37.0f, 47.0f};
  text2.spacing = 0.5f;
  text2.Set("pixel-font", "Start", {10, 100, 98});

  auto entityt3 = reg.create();
  auto &text3 = reg.emplace<Text>(entityt3);
  text3.spacing = 0.4f;
  text3.letterSize = {20.0f, 30.0f};
  text3.Set("pixel-font", "Avoid Karens and try to pick up", {10, -100, 98});

  auto entityt4 = reg.create();
  auto &text4 = reg.emplace<Text>(entityt4);
  text4.spacing = 0.4f;
  text4.letterSize = {20.0f, 30.0f};
  text4.Set("pixel-font", "as many groceries as possible !", {10, -150, 98});
}

void pandemic_shop::SetupRestartScreen(Engine &engine_,
                                       String number_of_collected_items_,
                                       String number_of_items_
    ) {
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

    auto entityt = reg.create();
    auto &text = reg.emplace<Text>(entityt);
    text.spacing = 0.6f;
    text.alignment = TextAlignment::CENTER;
    text.letterSize = {37.0f, 47.0f};

    //Game over
    //e sad ovo su stringovi, tako da vrv treba drugacije da se porede
    //if(number_of_collected_items_!=number_if_items){               <-----------------------
    //text.Set("pixel-font", "Game Over!", {10, 175, 98});           <-----------------------

    //pobeda
    //else{                                                          <-----------------------
    text.Set("pixel-font", "Victory!", {10, 175, 98});

    auto entityt1 = reg.create();
    auto &text1 = reg.emplace<Text>(entityt1);
    //Collected 7/11 (na primer)
    text1.spacing = 0.6f;
    text1.letterSize = {37.0f, 47.0f};
    text1.Set("pixel-font", "Collected "+number_of_collected_items_+"/"+number_of_items_ + " items", {10, 100, 98});

    //ovo ako nam bude zatrebalo da pisemo sa ovim formatom
    /*text.Set("pixel-font",
             fmt::format("Raid: {}s left!", (UInt32)player.timeLeft),
             {10, 260, 98});*/
}

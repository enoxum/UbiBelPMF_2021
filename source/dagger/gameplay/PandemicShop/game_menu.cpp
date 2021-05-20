#include "game_menu.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/sprite.h"
#include "core/graphics/window.h"
#include "core/input/inputs.h"

#include "gameplay/PandemicShop/pandemic_shop_main.h"

using namespace pandemic;
using namespace dagger;

bool GameMenuSystem::s_GameOver = false;
String GameMenuSystem::s_Winner = "";

void GameMenuSystem::Run() {
  auto &view = Engine::Registry().view<GameMenuButton, Sprite, InputReceiver>();

  for (auto entity : view) {
    auto &gmb = view.get<GameMenuButton>(entity);
    auto &sprite = view.get<Sprite>(entity);
    auto &input = view.get<InputReceiver>(entity);

    if (IsMouseOver(gmb)) {
      if (gmb.type == EGameMenuType::StartScreen) {
        AssignSprite(sprite, "PandemicShop:start_button");
      } else if (gmb.type == EGameMenuType::RestartScreen) {
        AssignSprite(sprite, "PandemicShop:tree1");
      }
      sprite.size = gmb.size;
      if (EPSILON_NOT_ZERO(input.Get("start"))) {
        m_LoadGame = true;
      }
    } else {
      if (gmb.type == EGameMenuType::StartScreen) {
        AssignSprite(sprite, "PandemicShop:start_button");
      } else if (gmb.type == EGameMenuType::RestartScreen) {
        AssignSprite(sprite, "PandemicShop:tree1");
      }
      sprite.size = gmb.size;
    }
  }
}

void GameMenuSystem::SpinUp() {
  Engine::Dispatcher().sink<NextFrame>().connect<&GameMenuSystem::OnEndOfFrame>(
      this);
}

void GameMenuSystem::WindDown() {
  Engine::Dispatcher()
      .sink<NextFrame>()
      .disconnect<&GameMenuSystem::OnEndOfFrame>(this);
}

void GameMenuSystem::OnEndOfFrame() {
  if (m_LoadGame) {
    m_LoadGame = false;
    Engine::Registry().clear();
  }
  if (s_GameOver) {
    s_GameOver = false;
    Engine::Registry().clear();
  }
}

bool GameMenuSystem::IsMouseOver(GameMenuButton gmb_) {
  auto cursor = dagger::Input::CursorPositionInWorld();
  Vector2 b(gmb_.position.x + gmb_.size.x * (-0.5),
            gmb_.position.y + gmb_.size.y * (-0.5));

  if (cursor.x < b.x + gmb_.size.x && cursor.x > b.x &&
      cursor.y < b.y + gmb_.size.y && cursor.y > b.y) {
    return true;
  }

  return false;
}

void GameMenuSystem::EndOfGame(String winner_) {
  s_GameOver = true;
  s_Winner = winner_;
}
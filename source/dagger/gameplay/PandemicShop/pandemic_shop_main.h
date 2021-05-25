#pragma once

#include "core/core.h"
#include "core/engine.h"
#include "core/game.h"
#include "core/system.h"

using namespace dagger;

namespace pandemic_shop {

void SetupWorld(Engine &engine_);
void SetupStartScreen(Engine &engine_);
void SetupRestartScreen(Engine &engine_, String number_of_collected_items_, String number_of_items_);

class PandemicShopGame : public Game {
  inline String GetIniFile() override { return "pingpong.ini"; };

  void CoreSystemsSetup(Engine &engine_) override;
  void GameplaySystemsSetup(Engine &engine_) override;
  void WorldSetup(Engine &engine_) override;
};
} // namespace pandemic_shop

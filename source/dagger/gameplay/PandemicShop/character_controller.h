#pragma once

#include "core/core.h"
#include "core/system.h"
#include "gameplay/PandemicShop/character_controller_fsm.h"

using namespace dagger;

namespace pandemic {
struct PandemicCharacter {
  int speed{1};
  EDirection direction{ EDirection::Down };
};

class PandemicControllerSystem : public System {
  CharacterControllerFSM characterFSM;

public:
  String SystemName() override { return "Character Controller System"; }

  void Run() override;
};
} // namespace platformer
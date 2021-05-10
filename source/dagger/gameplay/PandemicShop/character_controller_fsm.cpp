#include "character_controller_fsm.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/animation.h"
#include "core/graphics/sprite.h"
#include "core/input/inputs.h"
#include "gameplay/PandemicShop/character_controller.h"
#include "gameplay/PandemicShop/movable.h"

using namespace dagger;

// Idle

void CharacterControllerFSM::Idle::Enter(
    CharacterControllerFSM::StateComponent &state_) {
  auto &animator = Engine::Registry().get<Animator>(state_.entity);
  auto &movable = Engine::Registry().get<pandemic::Movable>(state_.entity);
  printf("idle: %d\n", movable.pravac);
  if (movable.pravac == 1) 
  {
    AnimatorPlay(animator, "PandemicShop:IDLE_FRONT");
  }
  else if (movable.pravac == 2) 
  {
    AnimatorPlay(animator, "PandemicShop:IDLE_RIGHT");
  }
  else if (movable.pravac == 3) 
  {
    AnimatorPlay(animator, "PandemicShop:IDLE_LEFT");
  } 
  else if (movable.pravac == 4) 
  {
    AnimatorPlay(animator, "PandemicShop:IDLE_DOWN");
  }
}

DEFAULT_EXIT(CharacterControllerFSM, Idle);

void CharacterControllerFSM::Idle::Run(
    CharacterControllerFSM::StateComponent &state_) {
  auto &input = Engine::Registry().get<InputReceiver>(state_.entity);

  if (EPSILON_NOT_ZERO(input.Get("run"))) {
    GoTo(ECharacterStates::Running, state_);
  }
}

// Running

void CharacterControllerFSM::Running::Enter(
    CharacterControllerFSM::StateComponent &state_) {
  auto &animator = Engine::Registry().get<Animator>(state_.entity);
  auto &movable = Engine::Registry().get<pandemic::Movable>(state_.entity);
  printf("run: %d\n", movable.pravac);
    if (movable.pravac == 1) {
    AnimatorPlay(animator, "PandemicShop:RUN_FRONT");
    } 
    else if (movable.pravac == 2) 
    {
    AnimatorPlay(animator, "PandemicShop:RUN_RIGHT");
    } 
    else if (movable.pravac == 3) 
    {
    AnimatorPlay(animator, "PandemicShop:RUN_LEFT");
    }
    else if (movable.pravac == 4) 
    {
    AnimatorPlay(animator, "PandemicShop:RUN_DOWN");
    }
}

// same as: DEFAULT_EXIT(CharacterControllerFSM, Running);
void CharacterControllerFSM::Running::Exit(
    CharacterControllerFSM::StateComponent &state_) {}

void CharacterControllerFSM::Running::Run(
    CharacterControllerFSM::StateComponent &state_) {
  auto &&[sprite, input, character] =
      Engine::Registry()
          .get<Sprite, InputReceiver, pandemic::PandemicCharacter>(
              state_.entity);

  Float32 run = input.Get("run");

  if (EPSILON_ZERO(run)) {
    GoTo(ECharacterStates::Idle, state_);
  } else {
    //sprite.scale.x = run;
    sprite.position.x += character.speed * sprite.scale.x * Engine::DeltaTime();
  }
}

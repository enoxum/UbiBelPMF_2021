#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"

enum struct ECharacterStates { Idle, Running };
enum struct EDirection { Up = 1, Right = 2, Left = 3, Down = 4 };

static inline EDirection GetDirectionFromVector(const Vector2 hv)
{
    if (hv.x == 0 && hv.y == -1) return EDirection::Up;
    else if (hv.x == 1 && hv.y == 0) return EDirection::Right;
    else if (hv.x == -1 && hv.y == 0) return EDirection::Left;
    else if (hv.x == 0 && hv.y == 1) return EDirection::Down;
}

struct CharacterControllerFSM : public FSM<ECharacterStates> {
  DEFINE_STATE(CharacterControllerFSM, ECharacterStates, Idle);
  DEFINE_STATE(CharacterControllerFSM, ECharacterStates, Running);

  CharacterControllerFSM() {
    CONNECT_STATE(ECharacterStates, Idle);
    CONNECT_STATE(ECharacterStates, Running);
  }
};

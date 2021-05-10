#include "gameplay/PandemicShop/character_controller_fsm.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/animation.h"
#include "core/graphics/sprite.h"
#include "core/input/inputs.h"
#include "gameplay/PandemicShop/character_controller.h"

using namespace dagger;
// Idle

void CharacterControllerFSM::Idle::Enter(
    CharacterControllerFSM::StateComponent &state_) {

    auto& [animator, character] = Engine::Registry().get<Animator, pandemic::PandemicCharacter>(state_.entity);

    String animationToPlay = "PandemicShop:IDLE_FRONT";
    switch (character.direction)
    {
    case EDirection::Up: animationToPlay = "PandemicShop:IDLE_FRONT"; break;
    case EDirection::Right: animationToPlay = "PandemicShop:IDLE_RIGHT"; break;
    case EDirection::Left: animationToPlay = "PandemicShop:IDLE_LEFT"; break;
    case EDirection::Down: default:
        animationToPlay = "PandemicShop:IDLE_DOWN"; break;
    }
    AnimatorPlay(animator, animationToPlay);
}

DEFAULT_EXIT(CharacterControllerFSM, Idle);

void CharacterControllerFSM::Idle::Run(CharacterControllerFSM::StateComponent &state_) {
    
    auto& input = Engine::Registry().get<InputReceiver>(state_.entity);

    auto h = input.Get("horizontal");
    auto v = input.Get("vertical");

    if (EPSILON_NOT_ZERO(h) || EPSILON_NOT_ZERO(v)) {
        GoTo(ECharacterStates::Running, state_);
    }
}

// Running

void CharacterControllerFSM::Running::Enter(
    CharacterControllerFSM::StateComponent &state_) {}

// same as: DEFAULT_EXIT(CharacterControllerFSM, Running);
void CharacterControllerFSM::Running::Exit(
    CharacterControllerFSM::StateComponent &state_) {}

void CharacterControllerFSM::Running::Run(
    CharacterControllerFSM::StateComponent &state_) 
{
    auto &&[animator, input, character] =
        Engine::Registry().get<Animator, InputReceiver, pandemic::PandemicCharacter>(state_.entity);

    auto h = input.Get("horizontal");
    auto v = input.Get("vertical");

    if (EPSILON_ZERO(h) && EPSILON_ZERO(v)) 
    {
        GoTo(ECharacterStates::Idle, state_);
    } else {
        auto direction = GetDirectionFromVector(Vector2{ h, v });
        if (direction != character.direction)
        {
            character.direction = direction;
            String animationToPlay = "PandemicShop:RUN_FRONT";
            switch (character.direction)
            {
            case EDirection::Up: animationToPlay = "PandemicShop:RUN_FRONT"; break;
            case EDirection::Right: animationToPlay = "PandemicShop:RUN_RIGHT"; break;
            case EDirection::Left: animationToPlay = "PandemicShop:RUN_LEFT"; break;
            case EDirection::Down: default: animationToPlay = "PandemicShop:RUN_DOWN"; break;
            }
            AnimatorPlay(animator, animationToPlay);
        }
    }
}

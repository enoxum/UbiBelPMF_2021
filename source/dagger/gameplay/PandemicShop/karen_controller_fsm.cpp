#include "gameplay/PandemicShop/karen_controller_fsm.h"
#include "gameplay/PandemicShop/karen_controller.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/animation.h"
#include "core/graphics/sprite.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"
#include "gameplay/PandemicShop/ai_system.h"
using namespace dagger;
using namespace pandemic;

// Idle

void KarenControllerFSM::Idle::Enter(
    KarenControllerFSM::StateComponent &state_) {

    auto&& [animator, character] = Engine::Registry().get<Animator, pandemic::PandemicKarenCharacter>(state_.entity);
    AnimatorPlay(animator, "PandemicShop:AMELIA_IDLE" + GetNameFromDirection(character.direction));
}

void KarenControllerFSM::Idle::Run(KarenControllerFSM::StateComponent &state_) {
	
    auto &command = Engine::Registry().get<AICommand>(state_.entity); 
    if (command.curr_action != Action::IDLE && command.curr_action != Action::PICK) {
        GoTo(EKarenStates::Running, state_);
    }
}

DEFAULT_EXIT(KarenControllerFSM, Idle);

// Running

void KarenControllerFSM::Running::Run(
    KarenControllerFSM::StateComponent &state_) 
{
  auto &&[animator, command, character, transform] =
        Engine::Registry().get<Animator, AICommand/*KarenAI*/, pandemic::PandemicKarenCharacter, Transform>(state_.entity);

    float up_down =0;
    float left_right = 0;
    // Logger::info("\nPOSITION BEFORE MOVING: {} {}\n", transform.position.x, transform.position.y);
    switch (command.curr_action)
    {
    
    case Action::LEFT:
      left_right = -1;
      transform.position.x += character.speed * (-1) * Engine::DeltaTime();
      command.current.x = transform.position.x;
      break;
    case Action::RIGHT:
      left_right = 1;
      transform.position.x += character.speed * (1) * Engine::DeltaTime();
      command.current.x = transform.position.x;
      break;
    case Action::UP:
      up_down = -1;
      transform.position.y += character.speed * (-1) * Engine::DeltaTime();
      command.current.y = transform.position.y;
      break;
    case Action::DOWN:
      up_down = 1;
      transform.position.y += character.speed * (1) * Engine::DeltaTime();
      command.current.y = transform.position.y;
      break;

    default:
      break;
    }

    auto dist = glm::length(Vector2(command.next.x - command.current.x, command.next.y - command.current.y));    
    Logger::info("\nDIRECTION VECTOR {} {}\n", left_right, up_down);
    
    if(abs(dist) < 1 || (EPSILON_ZERO(command.next.x - command.current.x) 
        && EPSILON_ZERO(command.next.y - command.current.y))){
      command.curr_action = Action::IDLE;
      command.previous = command.current;
      command.current = command.next;
      command.finished = true;
      GoTo(EKarenStates::Idle, state_);

    }
    else if (EPSILON_ZERO(left_right) && EPSILON_ZERO(up_down)) 
    {
        GoTo(EKarenStates::Idle, state_);
    }
     else {
        auto direction = GetDirectionFromVector(Vector2{ left_right, up_down});

            character.direction = direction;
            AnimatorPlay(animator, "PandemicShop:AMELIA_RUN" + GetNameFromDirection(direction));
    }
}

void KarenControllerFSM::Running::Enter(
    KarenControllerFSM::StateComponent& state_) {}

void KarenControllerFSM::Running::Exit(
    KarenControllerFSM::StateComponent& state_) {}

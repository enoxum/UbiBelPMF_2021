#include "roboship_character_controller_fsm.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "gameplay/roboship/roboship_controller.h"

using namespace dagger;

// Idle


void RoboshipCharacterControllerFSM::Idle::Enter(RoboshipCharacterControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "robot:IDLE");
}

DEFAULT_EXIT(RoboshipCharacterControllerFSM, Idle);

void RoboshipCharacterControllerFSM::Idle::Run(RoboshipCharacterControllerFSM::StateComponent& state_)
{
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);

	if (EPSILON_NOT_ZERO(input.Get("run")))
	{
		GoTo(EStates::Running, state_);
	}
}


// Running

void RoboshipCharacterControllerFSM::Running::Enter(RoboshipCharacterControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "robot:RUN");
}

// same as: DEFAULT_EXIT(CharacterControllerFSM, Running);
void RoboshipCharacterControllerFSM::Running::Exit(RoboshipCharacterControllerFSM::StateComponent& state_)
{}

void RoboshipCharacterControllerFSM::Running::Run(RoboshipCharacterControllerFSM::StateComponent& state_)
{
	auto&& [sprite, input, character] = Engine::Registry().get<Sprite, InputReceiver, roboship::RoboshipCharacter>(state_.entity);

	Float32 run = input.Get("run");

	if (EPSILON_ZERO(run))
	{
		GoTo(EStates::Idle, state_);
	}
	else
	{
		sprite.scale.x = run*0.1f;
		sprite.position.x += character.speed * run * Engine::DeltaTime();
	}
}

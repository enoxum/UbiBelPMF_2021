#include "char_controller_fsm.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "gameplay/platformer/platformer_controller.h"

using namespace dagger;

// Idle

void CharControllerFSM::Idle::Enter(CharControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "BlueWizard:IDLE");
}

DEFAULT_EXIT(CharControllerFSM, Idle);

void CharControllerFSM::Idle::Run(CharControllerFSM::StateComponent& state_)
{
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);

	if (EPSILON_NOT_ZERO(input.Get("walk")))
	{
		GoTo(ECharStates::Walking, state_);
	}

	if (EPSILON_NOT_ZERO(input.Get("jump")))
	{
		GoTo(ECharStates::Jumping, state_);
	}

	if (EPSILON_NOT_ZERO(input.Get("dash")))
	{
		GoTo(ECharStates::Dashing, state_);
	}
}


// Walking

void CharControllerFSM::Walking::Enter(CharControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "BlueWizard:WALK");
}

// same as: DEFAULT_EXIT(CharacterControllerFSM, Walking);
void CharControllerFSM::Walking::Exit(CharControllerFSM::StateComponent& state_)
{}

void CharControllerFSM::Walking::Run(CharControllerFSM::StateComponent& state_)
{
	auto&& [sprite, input, character] = Engine::Registry().get<Sprite, InputReceiver, platformer::PlatformerCharacter>(state_.entity);

	Float32 walk = input.Get("walk");

	if (EPSILON_ZERO(walk))
	{
		GoTo(ECharStates::Idle, state_);
	}
	else
	{
		sprite.scale.x = walk;
		sprite.position.x += character.speed * sprite.scale.x * Engine::DeltaTime();
	}

	if (EPSILON_NOT_ZERO(input.Get("jump")))
	{
		GoTo(ECharStates::Jumping, state_);
	}

	if (EPSILON_NOT_ZERO(input.Get("dash")))
	{
		GoTo(ECharStates::Dashing, state_);
	}
}

// Jumping

void CharControllerFSM::Jumping::Enter(CharControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "BlueWizard:JUMP");
}

DEFAULT_EXIT(CharControllerFSM, Jumping);

void CharControllerFSM::Jumping::Run(CharControllerFSM::StateComponent& state_)
{
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);

	// TODO

	
	if (EPSILON_NOT_ZERO(input.Get("dash")))
	{
		GoTo(ECharStates::Dashing, state_);
	}
}

// Dashing

void CharControllerFSM::Dashing::Enter(CharControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "BlueWizard:DASH");
}

DEFAULT_EXIT(CharControllerFSM, Dashing);

void CharControllerFSM::Dashing::Run(CharControllerFSM::StateComponent& state_)
{
	// TODO
}
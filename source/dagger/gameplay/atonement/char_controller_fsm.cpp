#include "char_controller_fsm.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"
#include <gameplay/atonement/atonement_controller.h>

#include "gameplay/atonement/systems/cooldown_manager.h"

#include <iostream>

using namespace dagger;
using namespace atonement;

// Idle

void CharControllerFSM::Idle::Enter(CharControllerFSM::StateComponent& state_)
{
	std::cout << "[IDLE]" << std::endl;
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
		GoTo(ECharStates::JumpWindup, state_);
	}

	if (EPSILON_NOT_ZERO(input.Get("dash")) && canDash(state_.entity))
	{
		GoTo(ECharStates::Dashing, state_);
	}

	auto& character = Engine::Registry().get<AtonementController::AtonementCharacter>(state_.entity);
	if (!character.grounded) {
		GoTo(ECharStates::JumpWinddown, state_);
	}
}


// Walking

void CharControllerFSM::Walking::Enter(CharControllerFSM::StateComponent& state_)
{
	std::cout << "[WALK]" << std::endl;
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "BlueWizard:WALK");
}

// same as: DEFAULT_EXIT(CharacterControllerFSM, Walking);
void CharControllerFSM::Walking::Exit(CharControllerFSM::StateComponent& state_)
{}

void CharControllerFSM::Walking::Run(CharControllerFSM::StateComponent& state_)
{
	auto&& [transform, sprite, input, character] = Engine::Registry().get<Transform, Sprite, InputReceiver, AtonementController::AtonementCharacter>(state_.entity);

	Float32 walk = input.Get("walk");

	if (EPSILON_ZERO(walk))
	{
		GoTo(ECharStates::Idle, state_);
	}
	else
	{
		if((sprite.scale.x < 0 && walk > 0) || (sprite.scale.x > 0 && walk < 0)){
			sprite.scale.x *= -1;
		}
		transform.position.x += character.speed * sprite.scale.x * Engine::DeltaTime();
	}

	if (EPSILON_NOT_ZERO(input.Get("jump")))
	{
		GoTo(ECharStates::JumpWindup, state_);
	}

	if (EPSILON_NOT_ZERO(input.Get("dash")) && canDash(state_.entity))
	{
		GoTo(ECharStates::Dashing, state_);
	}

	if (!character.grounded) {
		GoTo(ECharStates::JumpWinddown, state_);
	}
}

// Jump Windup

void CharControllerFSM::JumpWindup::Enter(CharControllerFSM::StateComponent& state_)
{
	std::cout << "[JUMP_WU]" << std::endl;
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlayOnce(animator, "BlueWizard:JUMP_WINDUP");
}

DEFAULT_EXIT(CharControllerFSM, JumpWindup);

void CharControllerFSM::JumpWindup::Run(CharControllerFSM::StateComponent& state_)
{
	auto&& [transform, sprite, input, character] = Engine::Registry().get<Transform, Sprite, InputReceiver, AtonementController::AtonementCharacter>(state_.entity);

	if (EPSILON_NOT_ZERO(input.Get("dash")) && canDash(state_.entity))
	{
		character.dashJumped = true;
		GoTo(ECharStates::Dashing, state_);
	}

	Float32 walk = input.Get("walk");

	if (!(EPSILON_ZERO(walk))) {
		if ((sprite.scale.x < 0 && walk > 0) || (sprite.scale.x > 0 && walk < 0)) {
			sprite.scale.x *= -1;
		}
		transform.position.x += character.speed * sprite.scale.x * Engine::DeltaTime();
	}

	if (character.jumpedHeight < character.jumpHeight) {
		Float32 tmp = character.jumpSpeed * sprite.scale.y * Engine::DeltaTime();
		transform.position.y += tmp;
		character.jumpedHeight += tmp;
	}
	else {
		character.jumpedHeight = 0;
		GoTo(ECharStates::JumpWinddown, state_);
	}
}

// Jump Winddown

void CharControllerFSM::JumpWinddown::Enter(CharControllerFSM::StateComponent& state_)
{
	std::cout << "[JUMP_WD]" << std::endl;
	auto&& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlayOnce(animator, "BlueWizard:JUMP_WINDDOWN");
	
	auto&& character = Engine::Registry().get<AtonementController::AtonementCharacter>(state_.entity);

	character.fallingAnimationEnded = false;
}

DEFAULT_EXIT(CharControllerFSM, JumpWinddown);

void CharControllerFSM::JumpWinddown::Run(CharControllerFSM::StateComponent& state_)
{
	static Float32 fallenInLastFrame = 0;

	auto&& [transform, sprite, input, character] = Engine::Registry().get<Transform, Sprite, InputReceiver, AtonementController::AtonementCharacter>(state_.entity);

	if (EPSILON_NOT_ZERO(input.Get("dash")) && canDash(state_.entity))
	{
		character.dashJumped = true;
		GoTo(ECharStates::Dashing, state_);
	}

	Float32 walk = input.Get("walk");

	if (!(EPSILON_ZERO(walk))) {
		if ((sprite.scale.x < 0 && walk > 0) || (sprite.scale.x > 0 && walk < 0)) {
			sprite.scale.x *= -1;
		}
		transform.position.x += character.speed * sprite.scale.x * Engine::DeltaTime();
	}

	if (!character.grounded) {
		fallenInLastFrame = character.fallSpeed * sprite.scale.y * Engine::DeltaTime();
		transform.position.y -= fallenInLastFrame;
	}
	else if (character.fallingAnimationEnded) {
		transform.position.y += fallenInLastFrame;
		fallenInLastFrame = 0;
		GoTo(ECharStates::Idle, state_);
	}
}

// Dashing

void CharControllerFSM::Dashing::Enter(CharControllerFSM::StateComponent& state_)
{
	std::cout << "[DASH]" << std::endl;
	auto&& [character, animator] = Engine::Registry().get<AtonementController::AtonementCharacter, Animator>(state_.entity);
	AnimatorPlayOnce(animator, "BlueWizard:DASH");

	character.dashingAnimationEnded = false;

	auto cdManager = Engine::GetDefaultResource<CooldownManager>();
	cdManager->registerCooldown(state_.entity, "dash", character.dashCooldown);
}

DEFAULT_EXIT(CharControllerFSM, Dashing);

void CharControllerFSM::Dashing::Run(CharControllerFSM::StateComponent& state_)
{
	auto&& [transform, sprite, character] = Engine::Registry().get<Transform, Sprite, AtonementController::AtonementCharacter>(state_.entity);
	
	transform.position.x += character.dashSpeed * sprite.scale.x * Engine::DeltaTime();

	if (character.dashingAnimationEnded) {
		if (!character.grounded) {
			GoTo(ECharStates::JumpWinddown, state_);
		}
		else {
			GoTo(ECharStates::Idle, state_);
		}
	}

}

/* Helper functions */

void CharControllerFSM::OnAnimationEnd(ViewPtr<Animation> animation) {
	Engine::Registry().view<CharControllerFSM::StateComponent>()
		.each([&](CharControllerFSM::StateComponent& state_)
			{
				String animName = animation.get()->name;
				auto&& character = Engine::Registry().get<AtonementController::AtonementCharacter>(state_.entity);

				if (animName == "BlueWizard:DASH") {
					character.dashingAnimationEnded = true;
				}
				else if (animName == "BlueWizard:JUMP_WINDDOWN") {
					character.fallingAnimationEnded = true;
				}
			});
}

Bool CharControllerFSM::canDash(Entity entity) {
	auto cdManager = Engine::GetDefaultResource<CooldownManager>();
	auto&& character = Engine::Registry().get<AtonementController::AtonementCharacter>(entity);
	return !cdManager->isOnCooldown(entity, "dash") && !character.dashJumped;
}
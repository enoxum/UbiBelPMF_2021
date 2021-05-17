#include "gameplay/roboship/roboship_controller.h"
#include "roboship_character_controller_fsm.h"
#include "gameplay/roboship/roboship_createbackdrop.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"


using namespace dagger;

static int count = 0;
static int iteration = 1;
static int iteration1 = 1;

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

	if (sprite.scale.x * run < 0) {
		Engine::Dispatcher().trigger<RChangeDirection>();
	}

	if (EPSILON_ZERO(run))
	{
		GoTo(EStates::Idle, state_);
	}
	else
	{
		if (iteration1 == 1)
		{
			iteration1++;
		}

		sprite.scale.x = run * 0.2f;
		sprite.position.x += character.speed * run * Engine::DeltaTime();

		printf("%lf\n", sprite.position.x);
		if (abs(sprite.position.x - 800) <= 0.3 && iteration == 1)
		{
			iteration++;
			count += 1600;
			puts("updated");
			RBackdrop::RoboshipCreateBackdrop(count, sprite.position.x);
		}
		else if (abs((sprite.position.x - 800) - (iteration - 1)*2000)-400*(iteration-1) <= 0.3 && iteration > 1) {
			iteration++;
			count += 1600;
			puts("updated");
			RBackdrop::RoboshipCreateBackdrop(count, sprite.position.x);
		}
	}
}

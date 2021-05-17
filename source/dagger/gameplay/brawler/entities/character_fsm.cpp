#include "character_fsm.h"

#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/animations.h"

#include "gameplay/common/simple_collisions.h"
#include "gameplay/brawler/components/bullet.h"
#include "gameplay/brawler/components/movable.h"
#include "gameplay/brawler/components/player.h"
#include "gameplay/brawler/systems/bullet_system.h"
#include "gameplay/brawler/systems/physics.h"
#include "gameplay/brawler/level.h"

using namespace dagger;
using namespace brawler;

// Idle

void BrawlerCharacterFSM::Idle::Enter(BrawlerCharacterFSM::StateComponent& state_)
{
	auto&& [player, animator] = Engine::Registry().get<Player, Animator>(state_.entity);
	AnimatorPlay(animator, "gunner:" + player.color + ":IDLE");
}

DEFAULT_EXIT(BrawlerCharacterFSM, Idle);

void BrawlerCharacterFSM::Idle::Run(BrawlerCharacterFSM::StateComponent& state_)
{
	auto&& [sprite, input, player, transform, movable, col] = Engine::Registry().get<Sprite, InputReceiver, Player, Transform, Movable, SimpleCollision>(state_.entity);

	if (!movable.isOnGround)
	{
		GoTo(ECharacterStates::Jumping, state_);
		return;
	}

	if (EPSILON_NOT_ZERO(input.Get("down")) && movable.canDrop) {
		transform.position.y -= Level::DROPDOWN_OFFSET;
		GoTo(ECharacterStates::Jumping, state_);
		return;
	}

	if (movable.isOnGround && EPSILON_NOT_ZERO(input.Get("jump"))) {
		movable.speed.y += PhysicsSystem::s_JumpSpeed;
		GoTo(ECharacterStates::Jumping, state_);
		return;
	}

	if (EPSILON_NOT_ZERO(input.Get("run")))
	{
		GoTo(ECharacterStates::Running, state_);
		return;
	}
}

// Running

void BrawlerCharacterFSM::Running::Enter(BrawlerCharacterFSM::StateComponent& state_)
{
	auto&& [player, animator] = Engine::Registry().get<Player, Animator>(state_.entity);
	AnimatorPlay(animator, "gunner:" + player.color + ":RUN");
}

DEFAULT_EXIT(BrawlerCharacterFSM, Running);

void BrawlerCharacterFSM::Running::Run(BrawlerCharacterFSM::StateComponent& state_)
{
	auto&& [sprite, input, player, transform, movable] = Engine::Registry().get<Sprite, InputReceiver, Player, Transform, Movable>(state_.entity);

	Float32 run = input.Get("run");

	if (!movable.isOnGround)
	{
		GoTo(ECharacterStates::Jumping, state_);
		return;
	}

	if (EPSILON_NOT_ZERO(input.Get("down")) && movable.canDrop) {
		transform.position.y -= Level::DROPDOWN_OFFSET;
		GoTo(ECharacterStates::Jumping, state_);
		return;
	}

	if (movable.isOnGround && EPSILON_NOT_ZERO(input.Get("jump")))
	{
		movable.speed.y += PhysicsSystem::s_JumpSpeed;
		GoTo(ECharacterStates::Jumping, state_);
		return;
	}

	if (EPSILON_ZERO(run))
	{
		GoTo(ECharacterStates::Idle, state_);
		return;
	} 
	else
	{
		sprite.scale.x = run;
		// if moving in opposite direction, first decrease x speed then move transform
		if(movable.speed.x * run < 0.0f)
		{
			movable.speed.x += run * PhysicsSystem::s_RunSpeed * Engine::DeltaTime();
			if(movable.speed.x * run > 0.0f)
			{
				transform.position.x += movable.speed.x;
				movable.speed.x = 0.0f;
			}
		}
		else
		{
			transform.position.x += run * PhysicsSystem::s_RunSpeed * Engine::DeltaTime();
		}
	}
	
}

// Jumping

void BrawlerCharacterFSM::Jumping::Enter(BrawlerCharacterFSM::StateComponent& state_)
{
	auto&& [player, animator] = Engine::Registry().get<Player, Animator>(state_.entity);
	AnimatorPlay(animator, "gunner:" + player.color + ":JUMP");
}

DEFAULT_EXIT(BrawlerCharacterFSM, Jumping);

void BrawlerCharacterFSM::Jumping::Run(BrawlerCharacterFSM::StateComponent& state_)
{
	auto&& [sprite, input, player, transform, movable] = Engine::Registry().get<Sprite, InputReceiver, Player, Transform, Movable>(state_.entity);

	Float32 run = input.Get("run");
	
	if (movable.isOnGround)
	{
		if (EPSILON_ZERO(run))
		{
			GoTo(ECharacterStates::Idle, state_);
		}
		else
		{
			GoTo(ECharacterStates::Running, state_);
		}
		return;
	}

 	auto jump = input.Get("jump");

	if(EPSILON_NOT_ZERO(jump))
	{
		// Jump longer by holding jump button
		movable.speed.y = movable.speed.y < PhysicsSystem::s_JumpSpeed/2 ? movable.speed.y : PhysicsSystem::s_JumpSpeed/2;
	}
		
	if (EPSILON_NOT_ZERO(run))
	{
		sprite.scale.x = run;
		transform.position.x += run * PhysicsSystem::s_AirMobility * PhysicsSystem::s_RunSpeed * Engine::DeltaTime();
	}

}
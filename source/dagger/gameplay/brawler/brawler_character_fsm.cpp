#include "brawler_character_fsm.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "gameplay/brawler/components/movable.h"
#include "gameplay/brawler/components/player.h"
#include "gameplay/brawler/systems/physics.h"
#include "gameplay/common/simple_collisions.h"
#include "core/graphics/animations.h"
#include <gameplay/brawler/components/bullet.h>
#include "gameplay/brawler/systems/bullet_system.h"

using namespace dagger;
using namespace brawler;

inline void playShootAnimation(BrawlerCharacterFSM::StateComponent& state_, Sprite& sprite, Movable& movable)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "Gunner_Green:SHOOT");
	movable.speed.x -= sprite.scale.x * recoil;
	
	
	// TODO: Refactor into `createBullet` or sth
	auto& reg = Engine::Registry();

	auto bulletEntity = reg.create();
	auto& bulletTransform = reg.emplace<Transform>(bulletEntity);
	auto& bulletSprite = reg.emplace<Sprite>(bulletEntity);
	bulletTransform.position = { sprite.position.x, sprite.position.y, 1 };
	AssignSprite(bulletSprite, "EmptyWhitePixel");
	bulletSprite.size = { 1, 1 };
	bulletSprite.scale = { 10, 10 };
	auto& bulletBullet = reg.emplace<Bullet>(bulletEntity);
	bulletBullet.direction = sprite.scale.x;

	BulletSystem::activeBullets++;
	// ----


}

// Idle

void BrawlerCharacterFSM::Idle::Enter(BrawlerCharacterFSM::StateComponent& state_)
{
	// TODO: cleanup
	auto&& [sprite, input, animator] = Engine::Registry().get<Sprite, InputReceiver, Animator>(state_.entity);
	AnimatorPlay(animator, "Gunner_Green:IDLE");

	
}

DEFAULT_EXIT(BrawlerCharacterFSM, Idle);

void BrawlerCharacterFSM::Idle::Run(BrawlerCharacterFSM::StateComponent& state_)
{
	auto&& [sprite, input, player, transform, movable, col] = Engine::Registry().get<Sprite, InputReceiver, Player, Transform, Movable, SimpleCollision>(state_.entity);


	if (EPSILON_NOT_ZERO(input.Get("fire_test"))) {

	}

	if (!movable.isOnGround)
	{
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

	if (EPSILON_NOT_ZERO(input.Get("attack")))
	{
		playShootAnimation(state_, sprite, movable);
	}
}

// Running

void BrawlerCharacterFSM::Running::Enter(BrawlerCharacterFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "Gunner_Green:RUN");
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

	if (movable.isOnGround && EPSILON_NOT_ZERO(input.Get("jump")))
	{
		movable.speed.y += PhysicsSystem::s_JumpSpeed;
		GoTo(ECharacterStates::Jumping, state_);
		return;
	}

	if (EPSILON_NOT_ZERO(input.Get("attack")))
	{
		playShootAnimation(state_, sprite, movable);
	}

	if (EPSILON_ZERO(run))
	{
		GoTo(ECharacterStates::Idle, state_);
		return;
	} 
	else
	{
		sprite.scale.x = run;
		transform.position.x += run * PhysicsSystem::s_RunSpeed * Engine::DeltaTime();
	}

	// TODO decrease x speed when running in opposite direction
}

// Jumping

void BrawlerCharacterFSM::Jumping::Enter(BrawlerCharacterFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "Gunner_Green:JUMP");
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

	auto attack = input.Get("attack");

	if(EPSILON_NOT_ZERO(attack))
	{
		playShootAnimation(state_, sprite, movable);
	}
}
#pragma once

#include "core/core.h"
#include "core/game/finite_state_machine.h"
#include "core/graphics/sprite.h"

#include "gameplay/brawler/components/movable.h"

using namespace dagger;

namespace brawler
{

enum struct ECharacterStates
{
	Idle,
	Running,
	Jumping,
	Attacking
};

struct BrawlerCharacterFSM : public FSM<ECharacterStates>
{
	DEFINE_STATE(BrawlerCharacterFSM, ECharacterStates, Idle);
	DEFINE_STATE(BrawlerCharacterFSM, ECharacterStates, Running);
	DEFINE_STATE(BrawlerCharacterFSM, ECharacterStates, Jumping);

	BrawlerCharacterFSM()
	{
		CONNECT_STATE(ECharacterStates, Idle);
		CONNECT_STATE(ECharacterStates, Running);
		CONNECT_STATE(ECharacterStates, Jumping);
	}

private:
	static void playShootAnimation(BrawlerCharacterFSM::StateComponent& state_, Sprite& sprite, Movable& movable);
};

}
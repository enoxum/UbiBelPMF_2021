#pragma once

#include "core/core.h"
#include "core/game/finite_state_machine.h"

using namespace dagger;

namespace brawler
{

static const int recoil = 25;

// class Movable;
// class Sprite;

// inline void playShootAnimation(BrawlerCharacterFSM::StateComponent& state_, Sprite& sprite, Movable& movable);

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
};

}
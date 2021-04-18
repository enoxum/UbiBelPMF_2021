#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"

enum struct ECharStates
{
	Idle,
	Walking,
	Jumping,
	Dashing
};

struct CharControllerFSM : public FSM<ECharStates>
{
	DEFINE_STATE(CharControllerFSM, ECharStates, Idle);
	DEFINE_STATE(CharControllerFSM, ECharStates, Walking);
	DEFINE_STATE(CharControllerFSM, ECharStates, Jumping);
	DEFINE_STATE(CharControllerFSM, ECharStates, Dashing);

	CharControllerFSM()
	{
		CONNECT_STATE(ECharStates, Idle);
		CONNECT_STATE(ECharStates, Walking);
		CONNECT_STATE(ECharStates, Jumping);
		CONNECT_STATE(ECharStates, Dashing);
	}
};

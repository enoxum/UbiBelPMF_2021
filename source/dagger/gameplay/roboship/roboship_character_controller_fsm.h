#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"

enum struct EStates
{
	Idle,
	Running
};

struct RoboshipCharacterControllerFSM : public FSM<EStates>
{
	DEFINE_STATE(RoboshipCharacterControllerFSM, EStates, Idle);
	DEFINE_STATE(RoboshipCharacterControllerFSM, EStates, Running);

	RoboshipCharacterControllerFSM()
	{
		CONNECT_STATE(EStates, Idle);
		CONNECT_STATE(EStates, Running);
	}
};

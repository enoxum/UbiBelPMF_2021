#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"

namespace atonement {

	enum struct ECharStates
	{
		Idle,
		Walking,
		JumpWindup,
		JumpWinddown,
		Dashing
	};

	struct CharControllerFSM : public FSM<ECharStates>
	{
		DEFINE_STATE(CharControllerFSM, ECharStates, Idle);
		DEFINE_STATE(CharControllerFSM, ECharStates, Walking);
		DEFINE_STATE(CharControllerFSM, ECharStates, JumpWindup);
		DEFINE_STATE(CharControllerFSM, ECharStates, JumpWinddown);
		DEFINE_STATE(CharControllerFSM, ECharStates, Dashing);

		CharControllerFSM()
		{
			CONNECT_STATE(ECharStates, Idle);
			CONNECT_STATE(ECharStates, Walking);
			CONNECT_STATE(ECharStates, JumpWindup);
			CONNECT_STATE(ECharStates, JumpWinddown);
			CONNECT_STATE(ECharStates, Dashing);
		}
	};

}
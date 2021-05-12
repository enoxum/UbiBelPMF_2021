#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/engine.h"
#include "gameplay/atonement/char_controller_fsm.h"

using namespace dagger;

namespace atonement
{


	class AtonementController
	{
	public:
		struct AtonementCharacter
		{
			Float32 speed{ 800 };
			Float32 jumpSpeed{ 1000 };
			Float32 jumpedHeight{ 0 };
			Float32 jumpHeight{ 300};
			Float32 dashSpeed{ 1600 };
			Float32 fallSpeed{ 900 };
			Float32 dashCooldown{ 1 };
			Bool grounded{ true };
			Bool fallingAnimationEnded{ false };
			Bool dashingAnimationEnded{ false };
			Bool dashJumped{ false };
		};

		class AtonementControllerSystem
			: public System
		{

		public:
			CharControllerFSM characterFSM;

			AtonementControllerSystem();

			String SystemName() override {
				return "Character Controller System";
			}

			void Run() override;
		};
	};

}
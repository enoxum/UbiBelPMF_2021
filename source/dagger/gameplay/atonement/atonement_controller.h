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
			Float32 speed{ 500 };
			Float32 jumpSpeed{ 300 };
			Float32 jumped{ 0 };
			Float32 jumpHeight{ 100 };
			Float32 dashSpeed{ 2000 };
			Float32 fallSpeed{ 400 };
			Bool grounded{ true };
		};

		class AtonementControllerSystem
			: public System
		{
			CharControllerFSM characterFSM;

		public:
			AtonementControllerSystem();

			String SystemName() override {
				return "Character Controller System";
			}

			void Run() override;
		};
	};

}
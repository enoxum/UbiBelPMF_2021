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
			int speed{ 1 };
		};

		class AtonementControllerSystem
			: public System
		{
			CharControllerFSM characterFSM;

		public:
			String SystemName() override {
				return "Character Controller System";
			}

			void Run() override;
		};
	};

}
#pragma once

#include "core/core.h"
#include "core/system.h"
#include "gameplay/roboship/roboship_main.h"
#include "gameplay/roboship/roboship_character_controller_fsm.h"

#include <vector>

using namespace dagger;

namespace roboship
{
	struct RoboshipCharacter
	{
		int speed{ 1 };
	};

	class RoboshipControllerSystem
		: public System
	{
		RoboshipCharacterControllerFSM characterFSM;

	public:
		String SystemName() override {
			return "Character Controller System";
		}

		void Run() override;
	};
}
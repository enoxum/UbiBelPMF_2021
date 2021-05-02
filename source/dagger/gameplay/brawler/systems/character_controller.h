#pragma once

#include "core/core.h"
#include "core/system.h"
#include "gameplay/brawler/entities/character_fsm.h"

using namespace dagger;

namespace brawler
{
	class CharacterControllerSystem
		: public System
	{
		BrawlerCharacterFSM characterFSM;

	public:
		String SystemName() override {
			return "Character Controller System";
		}

		void Run() override;
	};
}
#pragma once

#include <vector>

#include "core/core.h"
#include "core/system.h"

#include "gameplay/brawler/entities/weaponpickup.h"



using namespace dagger;

namespace brawler
{
    struct Player
	{
		bool placeholder{ false };

		// TODO: There should probably be a separate class called Weapon which contains WeaponType
		std::vector<WeaponType> weapons;
	};
}
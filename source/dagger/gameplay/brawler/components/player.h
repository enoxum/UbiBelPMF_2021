#pragma once

#include <vector>

#include "core/core.h"
#include "core/engine.h"

#include "gameplay/brawler/entities/weaponpickup.h"



using namespace dagger;

namespace brawler
{
	struct Player
	{

		int 					maxWeapons = 5;
		int 					pickedUpWeapons = 0;
		Vector2 				startPosition{ 0.0f, 0.0f };
		int 					startDirection{ 1 };
		std::vector<Weapon>		weapons;
		int						active_weapon_idx{ -1 };
		Entity 					currentWeapon;
	};
}
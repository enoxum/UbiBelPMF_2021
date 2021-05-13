#pragma once
#include "gameplay/brawler/weapon.h"
namespace brawler {

	struct WeaponPickup
	{
		Weapon weapon;
		TimePoint spawnTime;
	};
}


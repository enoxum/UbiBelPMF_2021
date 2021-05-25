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
		String 					name = "player";
		String					color = "green";
		bool					isLeft{ false };

		int						health{ 100 };

		// Weapons
		int 					maxWeapons = 5;
		int 					pickedUpWeapons = 0;
		Vector2 				startPosition{ 0.0f, 0.0f };
		int 					startDirection{ 1 };
		std::vector<Weapon>		weapons;
		int						active_weapon_idx{ -1 };
		Entity 					currentWeapon;


		bool dealDamage(int damage)
		{
			health -= damage;
			if (health < 0) {
				health = 0;
			}
			return health == 0;
		}

		void giveHealth(int heal)
		{
			health += heal;
			if (health > 100)
				health = 100;
		}
	};
}
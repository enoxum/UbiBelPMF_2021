#pragma once
namespace brawler {
	enum class WeaponType
	{
		PISTOL = 0,
		MACHINE_GUN = 1,
		HAND_GRENADE = 2
	};

	static const char* WeaponTypeName[] = { "Pistol", "Machine Gun", "Hand Grenade" };

	struct WeaponPickup
	{
		WeaponType weaponType;
		bool pickedUp{ false };
	};
}


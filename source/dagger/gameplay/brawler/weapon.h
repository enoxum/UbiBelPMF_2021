#pragma once

#include "core/core.h"

namespace brawler {
	enum class WeaponType
	{
		PISTOL      = 0,
		MACHINE_GUN = 1,
		M4A1 		= 2,
		AK 			= 3,
		P90 		= 4,
		SNIPER 		= 5,
		UZI 		= 6,
		BAZOOKA 	= 7,
		C4 			= 8,
		GRANADE 	= 9,
		MINE    	= 10,
		MOLOTOV 	= 11,
		MEDKIT 		= 12,
		BANANA 		= 13
	};

	class Weapon
	{
	public:
		static Weapon Pistol(int numClips)
		{
			return Weapon(WeaponType::PISTOL, "Pistol", 2, 20, 3, 7, numClips, "machinegun");
		}

		static Weapon MachineGun(int numClips)
		{
			return Weapon(WeaponType::MACHINE_GUN, "Machine Gun", 10, 15, 4, 70, numClips, "pistol");
		}

		static Weapon M4A1(int numClips)
		{
			return Weapon(WeaponType::M4A1, "M4A1",8, 15, 4, 30, numClips, "m4a1"); 
		}

		static Weapon AK(int numClips)
		{
			return Weapon(WeaponType::AK, "AK",8, 15, 4, 30, numClips, "ak"); 
		}

		static Weapon P90(int numClips)
		{
			return Weapon(WeaponType::P90, "P90",6, 12, 3, 50, numClips, "p90"); 
		}

		static Weapon Sniper(int numClips)
		{
			return Weapon(WeaponType::SNIPER, "Sniper",20, 20, 5, 5, numClips, "sniper"); 
		}

		static Weapon Uzi(int numClips)
		{
			return Weapon(WeaponType::UZI, "Uzi",6, 12, 3, 50, numClips, "uzi"); 
		}

		static Weapon CreateRandom() {
			WeaponType randomType = static_cast<WeaponType>(rand() % 7);
			switch (randomType) {
			case WeaponType::PISTOL:
				return Pistol(1 + rand() % 7);
			case WeaponType::MACHINE_GUN:
				return MachineGun(1 + rand() % 3);
			case WeaponType::M4A1:
				return M4A1(1 + rand() % 4);
			case WeaponType::AK:
				return AK(1 + rand() % 4);
			case WeaponType::P90:
				return P90(1 + rand() % 4);
			case WeaponType::SNIPER:
				return Sniper(1 + rand() % 2);
			case WeaponType::UZI:
				return Uzi(1 + rand() % 4);
			default:
				throw "Unknown weapon type!";
			}
		}

	public:
		Weapon(const WeaponType& weaponType,
			const String& name,
			int damage,
			int recoil,
			int bulletSize,
			int clipSize,
			int numClips,
			String sprite
				);
		Weapon();

		WeaponType weaponType() const;
		String  name() const;
		int		damage() const;
		int		recoil() const;
		int		bulletSize() const;
		int		clipSize() const;
		int		numClips() const;
		int		currentAmmoInClip() const;
		String  sprite() const;


		bool	reload();
		bool	shoot();

		bool	empty() const;

		bool	transferAmmo(Weapon& other);

		
	private:
		WeaponType m_weaponType;
		String	   m_name;
		int		   m_recoil;
		int		   m_damage;
		int		   m_bulletSize;
		int		   m_clipSize;
		int		   m_numClips;
		int		   m_currentAmmoInClip;
		String     m_sprite;

	};
}


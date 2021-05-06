#pragma once

#include "core/core.h"

namespace brawler {
	enum class WeaponType
	{
		PISTOL = 0,
		MACHINE_GUN = 1
	};

	class Weapon
	{
	public:
		static Weapon Pistol(int numClips)
		{
			return Weapon(WeaponType::PISTOL, "Pistol", 2, 20, 3, 7, numClips);
		}

		static Weapon MachineGun(int numClips)
		{
			return Weapon(WeaponType::MACHINE_GUN, "Machine Gun", 10, 15, 4, 70, numClips);
		}

		static Weapon CreateRandom() {
			WeaponType randomType = static_cast<WeaponType>(rand() % 2);
			switch (randomType) {
			case WeaponType::PISTOL:
				return Pistol(1 + rand() % 7);
			case WeaponType::MACHINE_GUN:
				return MachineGun(1 + rand() % 3);
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
			int numClips
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

	};
}


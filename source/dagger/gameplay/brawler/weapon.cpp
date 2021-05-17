#include "weapon.h"
namespace brawler {

Weapon::Weapon(const WeaponType& weaponType, 
			   const String& weaponName,
			   int damage,
			   int recoil, 
			   int bulletSize,
			   int clipSize,
			   int numClips,
			   String sprite,
			   Vector2 translate,
			   Vector2 scale)
	: m_weaponType(weaponType),
	  m_name(weaponName),
	  m_damage(damage),
	  m_recoil(recoil),
	  m_bulletSize(bulletSize),
	  m_clipSize(clipSize),
	  m_numClips(numClips),
	  m_currentAmmoInClip(clipSize),
	  m_sprite(sprite),
	  m_translate(translate),
	  m_scale(scale)
{
}

Weapon::Weapon()
{
}

WeaponType Weapon::weaponType() const
{
	return m_weaponType;
}

String Weapon::name() const
{
	return m_name;
}

int Weapon::damage() const
{
	return m_damage;
}

int Weapon::recoil() const
{
	return m_recoil;
}

int Weapon::bulletSize() const
{
	return m_bulletSize;
}

int Weapon::clipSize() const
{
	return m_clipSize;
}

int Weapon::numClips() const
{
	return m_numClips;
}

int Weapon::currentAmmoInClip() const
{
	return m_currentAmmoInClip;
}

String Weapon::ammoRepr() const
{
	return "x" + std::to_string(m_currentAmmoInClip) + " [" + std::to_string(m_numClips) + "]";
}

String Weapon::sprite() const
{
	return m_sprite;
}

Vector2 Weapon::translate() const
{
	return m_translate;
}

Vector2 Weapon::scale() const
{
	return m_scale;
}

bool Weapon::reload()
{
	if (m_currentAmmoInClip > 0)
		return false;
	if (m_numClips == 0)
		return false;
	m_numClips--;
	m_currentAmmoInClip = m_clipSize;
}

bool Weapon::shoot()
{
	if (m_currentAmmoInClip == 0)
		return false;
	m_currentAmmoInClip--;
	return true;
}

bool Weapon::empty() const
{
	return m_currentAmmoInClip == 0 && m_numClips == 0;
}

bool Weapon::transferAmmo(Weapon& other)
{
	if (m_weaponType != other.m_weaponType)
		return false;

	m_numClips += other.m_numClips;
	other.m_numClips = 0;

	return true;
}

}


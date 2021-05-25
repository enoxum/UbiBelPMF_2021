#pragma once

#include "core/core.h"
#include "core/system.h"

#include "gameplay/brawler/weapon.h"

using namespace dagger;
using namespace brawler;

struct Bullet
{
	Entity owner;
	int direction;
	int damage;
	bool projectile;
	WeaponType type;
	Float32 duration;
	bool timer;

	bool done{ false };
};
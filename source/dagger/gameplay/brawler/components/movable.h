#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace brawler
{

struct Movable
{
	Vector3 prevPosition{ 0, 0, 0 };

	Vector2 speed{ 0, 0 };
	Vector2 prevSpeed{ 0, 0 };

	Bool isOnGround{ true };
	Bool wasOnGround{ true };
};

}
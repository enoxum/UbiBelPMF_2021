#pragma once

#include "core/core.h"
#include "core/engine.h"

using namespace dagger;

namespace brawler
{
    struct Player
	{
		Vector2 startPosition{ 0.0f, 0.0f };
		int startDirection{ 1 };
	};
}
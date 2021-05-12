#pragma once

#include "core/core.h"
#include "core/engine.h"

using namespace dagger;

namespace brawler
{

constexpr Float32 TOTAL_COOLDOWN = 5.0f;

struct WeaponSpawner
{
    Vector2 position {0.0f, 0.0f};
    Entity weapon {entt::null};
    Float32 cooldown {TOTAL_COOLDOWN};
};

}
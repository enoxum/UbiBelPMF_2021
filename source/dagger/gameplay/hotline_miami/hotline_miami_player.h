#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace hotline_miami
{
   
    struct HotlineMiamiPlayer
    {
        bool is_player{ true };
        Float32 player_speed = 300.f;
    };

    class HotlineMiamiPlayerObstacleCollisionSystem
        : public System
    {
    public:
        inline String SystemName() { return "HotlineMiami Player Obstacle Collision System"; }

        void Run() override;
    };

    class HotlineMiamiPlayerShootingSystem
        : public System
    {
    public:
        inline String SystemName() { return "HotlineMiami Player Shooting System"; }

        void Run() override;
    };
}
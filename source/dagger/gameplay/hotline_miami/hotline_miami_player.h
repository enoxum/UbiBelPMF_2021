#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace hotline_miami
{
    struct HotlineMiamiPlayer
    {
        bool isPlayer{ true };
        Float32 s_PlayerSpeed;
    };


    class HotlineMiamiPlayerObstacleCollisionSystem
        : public System
    {
    public:
        inline String SystemName() { return "HotlineMiami Player Obstacle Collision System"; }

        void Run() override;
    };
}
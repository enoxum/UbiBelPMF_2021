#pragma once

#include "gameplay/hotline_miami/hotline_miami_player_input.h"

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace hotline_miami
{

    struct HotlineMiamiProjectile
    {

        Float32 projectile_speed = 40.f;
        Vector3 direction{ 0, 0, 0 };
    };

    class HotlineMiamiProjectileSystem
        : public System
    {
    public:

        inline String SystemName() { return "HotlineMiami Projectile System"; }

        void Run() override;

        static void SetupProjectileStats(HotlineMiamiProjectile& HotlineMiamiProjectile_, int look_direction)
        {
            HotlineMiamiProjectile_.projectile_speed = 600.f;

            if (look_direction == DOWN)
            {
                HotlineMiamiProjectile_.direction = { 0, -1, 0 };
            }
            else if (look_direction == TOP)
            {
                HotlineMiamiProjectile_.direction = { 0, 1, 0 };
            }
            else if (look_direction == LEFT)
            {
                HotlineMiamiProjectile_.direction = { -1, 0, 0 };
            }
            else if (look_direction == RIGHT)
            {
                HotlineMiamiProjectile_.direction = { 1, 0, 0 };
            }
        }
    };

    class HotlineMiamiProjectileObstacleCollisionSystem
        : public System
    {
    public:

        inline String SystemName() { return "HotlineMiami Projectile Obstacle Collision System"; }

        void Run() override;
       
    };
}
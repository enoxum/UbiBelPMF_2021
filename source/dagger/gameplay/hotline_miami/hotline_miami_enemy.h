#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace hotline_miami
{
    struct HotlineMiamiEnemy
    {
        bool is_enemy{ true };
        int weapon_type = 0;
        bool is_dead{ false };
    };

    struct HotlineMiamiEnemyBullet
    {
        Float32 bullet_speed = 300.f;
        Vector3 direction{ 0, 0, 0 };
        bool has_direction{ false };
    };

    class HotlineMiamiEnemyProjectileCollisionSystem
        : public System
    {
    public:
        inline String SystemName() { return "HotlineMiami Enemy Projectile Collision System"; }

        void Run() override;
    };

    class HotlineMiamiEnemyBulletSystem
        : public System
    {
    public:
        int interval = 10;

        inline String SystemName() { return "HotlineMiami Enemy Bullet System"; }

        static void SetupEnemyBulletStats(HotlineMiamiEnemyBullet& HotlineMiamiEnemyBullet_, Vector3 direction)
        {
            HotlineMiamiEnemyBullet_.direction = direction;
        }
        void Run() override;
    };

    class HotlineMiamiEnemyBulletObstacleCollisionSystem
        : public System
    {
    public:
        inline String SystemName() { return "HotlineMiami Enemy Bullet Obstacle Collision System"; }

        void Run() override;
    };

    class HotlineMiamiEnemyBulletPlayerCollisionSystem
        : public System
    {
    public:
        inline String SystemName() { return "HotlineMiami Enemy Bullet Player Collision System"; }

        void Run() override;
    };

}
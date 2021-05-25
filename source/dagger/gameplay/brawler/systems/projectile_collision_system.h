#pragma once
#include "core/system.h"
#include "core/core.h"

#include "gameplay/brawler/components/bullet.h"
#include "gameplay/brawler/components/movable.h"
#include "gameplay/brawler/components/player.h"


using namespace dagger;

namespace brawler 
{

    class ProjectileCollisionSystem : public System
    {
    public:
        inline String SystemName() { return "Projectile Collision System"; }

        

        static float s_grenadeSquaredRange;
        static float s_c4SquaredRange;

        float getDamageCoeff(const Vector3& playerPosition, const Vector3& projectilePosition, float range);

        bool explodePlayer(Player& player,
            Movable& playerMovable,
            Bullet& bullet,
            const Vector3& playerPosition,
            const Vector3& bulletPosition,
            float bulletSquareRange);

        void Run() override;
    };
}
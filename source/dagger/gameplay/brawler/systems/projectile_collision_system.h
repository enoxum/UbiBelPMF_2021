#pragma once
#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace brawler 
{
    class ProjectileCollisionSystem : public System
    {
    public:
        inline String SystemName() { return "Projectile Collision System"; }

        void Run() override;
    };
}
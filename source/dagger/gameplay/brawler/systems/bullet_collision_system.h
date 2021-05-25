#pragma once
#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace brawler 
{
    class BulletCollisionSystem : public System
    {
    public:
        inline String SystemName() { return "Bullet Collision System"; }

        void Run() override;
    };
}
#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace tanks
{
    struct TankBullet
    {
        Vector3 speed{ 0, 0, 0 };

        int damage{ 0 };
    };


    class TankBulletSystem
        : public System
    {
    public:

        inline String SystemName() { return "Tank Bullet System"; }

        void Run() override;
    };
}
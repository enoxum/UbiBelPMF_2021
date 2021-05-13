#pragma once
#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace brawler 
{
    class WeaponCollisionSystem : public System
    {
    public:
        inline String SystemName() { return "Weapon Collision System"; }

        void Run() override;
    };
}
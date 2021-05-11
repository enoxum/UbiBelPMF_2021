#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace brawler {


    class HUDSystem : public System
    {
    public:
        static float s_mainWeaponScale;
        static float s_otherWeaponsScale;

    public:

        String SystemName() override {
            return "HUD System";
        }

        void Run() override;
    };

}

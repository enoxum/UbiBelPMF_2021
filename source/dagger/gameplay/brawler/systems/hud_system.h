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
        static float s_blipWidth;
        static float s_blipHeight;

        static float s_initialYOffset;


        static void CreateHealthBar(bool side);
        static void CreateHealthBarLeft();
        static void CreateHealthBarRight();

        static void CreateWeaponsLeft();
        static void CreateWeaponsRight();
        static void CreateWeapons(bool side);

        static void CreateMainWeaponBlip(bool side);
        static void CreateLeftMainWeaponBlip();
        static void CreateRightMainWeaponBlip();

    public:

        String SystemName() override {
            return "HUD System";
        }

        void Run() override;


    };

}

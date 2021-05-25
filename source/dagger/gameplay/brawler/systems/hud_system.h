#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace brawler {

    class HUDSystem : public System
    {
    public:
        static float s_weaponDim;
        static float s_blipWidth;
        static float s_blipHeight;

        static float s_activeWeaponIndicatorSize;
 
        static float s_paddingUp;
        static float s_paddingSide;

        static float s_midPaddingUp;
        static float s_midPaddingSide;

        static      Entity leftMarker;
        static      Entity rightMarker;
        static      Entity leftText;
        static      Entity rightText;

        static void CreateHealthBar(bool side);
        static void CreateHealthBarLeft();
        static void CreateHealthBarRight();

        static void CreateWeaponsLeft();
        static void CreateWeaponsRight();
        static void CreateWeapons(bool side);

        static void CreateMarkersAndTexts();

    public:

        String SystemName() override {
            return "HUD System";
        }

        static void Init();
        void Run() override;

    };

}
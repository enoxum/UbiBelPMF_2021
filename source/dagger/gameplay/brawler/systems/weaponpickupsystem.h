#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace brawler {


    class WeaponPickupSystem : public System
    {
    public:

        String SystemName() override {
            return "Weapon Pickup System";
        }

        void Run() override;
    };

}

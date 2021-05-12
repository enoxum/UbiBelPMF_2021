#pragma once

#include "core/system.h"
#include "core/core.h"
#include "gameplay/brawler/weapon.h"

using namespace dagger;

namespace brawler {


class ShootingSystem : public System
{
public:

    String SystemName() override {
        return "Shooting System";
    }

    void Run() override;

    static Bool isProjectile(WeaponType wp);
    static void editSprite(Entity& wpSprite, Weapon& wp, double alpha);
};

}

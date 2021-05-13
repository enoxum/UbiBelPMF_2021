#pragma once

#include "core/core.h"
#include "core/engine.h"
#include "core/system.h"

using namespace dagger;

namespace brawler {

class WeaponSpawnSystem : public System
{
public:
    String SystemName() override {
        return "Weapon Spawn System";
    }

    void Run() override;

    void SpawnAll();

};

}
#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace brawler {


class ShootingSystem : public System
{
public:

    String SystemName() override {
        return "Shooting System";
    }

    void Run() override;
};

}

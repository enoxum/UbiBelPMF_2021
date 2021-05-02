#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace brawler {


class BulletSystem : public System
{
public:
    static int s_ActiveBullets;
    static float s_BulletSpeed;
    static float s_PlayerRecoil;

    static float s_CameraBoundLeft;
    static float s_CameraBoundRight;
    static float s_CameraBoundUp;
    static float s_CameraBoundDown;

    String SystemName() override {
        return "Bullet System";
    }

    void Run() override;
};

}

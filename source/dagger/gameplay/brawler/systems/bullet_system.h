#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace brawler {


class BulletSystem : public System
{
public:
    static float bulletSpeed;
    static int activeBullets;


    static float cameraBoundLeft;
    static float cameraBoundRight;
    static float cameraBoundUp;
    static float cameraBoundDown;
    String SystemName() override {
        return "Bullet System";
    }

    void Run() override;
};

}

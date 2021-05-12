#pragma once

#include "core/core.h"
#include "core/engine.h"

using namespace dagger;

namespace brawler {

class PhysicsSystem : public System
{
public:
    static bool s_UseGravity;
    static float s_Gravity;
    static float s_RunSpeed;
    static float s_JumpSpeed;
    static float s_TerminalVelocity;
    static float s_DragSpeed;
    static float s_AirMobility;

    String SystemName() override {
        return "Physics System";
    }

    void Run() override;

};

}
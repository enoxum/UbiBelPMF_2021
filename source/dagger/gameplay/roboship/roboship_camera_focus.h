#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace roboship
{
    struct RCameraFollowFocus
    {
        UInt32 weight{ 1 };
    };

    class RCameraFollowSystem
        : public System
    {
        inline String SystemName() override {
            return "Camera Follow System";
        };

        void Run() override;
    };
}
#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace hotline_miami
{
    struct HotlineMiamiCameraFollowFocus
    {
        UInt32 weight{ 1 };
    };

    class HotlineMiamiCameraFollowSystem
        : public System
    {
        inline String SystemName() override {
            return "HotlineMiami Camera System";
        };

        void Run() override;
    };
}
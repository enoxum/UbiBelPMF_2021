#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/graphics/sprite.h"

using namespace dagger;

namespace atonement
{
    struct Parallax
    {
        Vector2 lastCameraPosition{ 0, 0 };
        Float32 strength{ 0 };
    };

    class ParallaxSystem
        : public System
    {
        inline String SystemName() override {
            return "Parallax System";
        };

        void SpinUp() override;

        void Run() override;

    private:
        Sprite leftmost;
        Sprite rightmost;
    };
}
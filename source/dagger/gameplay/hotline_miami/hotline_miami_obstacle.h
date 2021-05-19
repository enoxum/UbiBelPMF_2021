#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace hotline_miami
{
    struct HotlineMiamiObstacle
    {
        bool isObstacle{ true };
    };

    class HotlineMiamiObstacleSystem
        : public System
    {
    public:

        inline String SystemName() { return "HotlineMiami Obstacle System"; }
    };
}
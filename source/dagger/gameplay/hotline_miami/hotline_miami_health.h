#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace hotline_miami
{

    struct HotlineMiamiHealth
    {
        bool is_health{ true };
    };

    class HotlineMiamiHealthSystem
        : public System
    {
    public:

        inline String SystemName() { return "HotlineMiami Health System"; }

        void Run() override;
    };
}
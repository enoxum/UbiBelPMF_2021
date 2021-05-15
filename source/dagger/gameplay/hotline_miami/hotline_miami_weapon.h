#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace hotline_miami
{

    struct HotlineMiamiDeleteEntity
    {
        String type{ "none" };
    };

    class HotlineMiamiWeaponSystem
        : public System
    {
    public:

        inline String SystemName() { return "HotlineMiami Weapon System"; }

        void Run() override;
    };
}
#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace hotline_miami
{

    struct HotlineMiamiWeapon
    {
        int type{ 0 };
    };

    struct HotlineMiamiKey
    {
        bool is_key{ true };
    };

    struct HotlineMiamiWorldEnd
    {
        bool the_end{ true };
    };

    class HotlineMiamiWeaponSystem
        : public System
    {
    public:

        inline String SystemName() { return "HotlineMiami Weapon System"; }

        void Run() override;
    };

    class HotlineMiamiKeySystem
        : public System
    {
    public:

        inline String SystemName() { return "HotlineMiami Key System"; }

        void Run() override;
    };

    class HotlineMiamiWorldEndSystem
        : public System
    {
    public:

        inline String SystemName() { return "HotlineMiami World End System"; }

        void Run() override;
    };
}
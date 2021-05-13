#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace hotline_miami
{

    struct HotlineMiamiDeleteEntity
    {
        bool to_be_deleted{ true };
    };

    class HotlineMiamiDeleteEntitySystem
        : public System
    {
    public:

        inline String SystemName() { return "HotlineMiami Delete Entity System"; }

        void Run() override;
    };
}
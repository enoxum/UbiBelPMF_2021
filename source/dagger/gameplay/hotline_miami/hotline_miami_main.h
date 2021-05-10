#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"

using namespace dagger;

namespace hotline_miami
{
    void SetupWorld();

    class HotlineMiamiGame : public Game
    {
        inline String GetIniFile() override
        {
            return "hotlinemiami.ini";
        };

        void CoreSystemsSetup() override;
        void GameplaySystemsSetup() override;
        void WorldSetup() override;
    };
}

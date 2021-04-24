#pragma once 

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"

using namespace dagger;

namespace tanks
{
    void SetupWorld();

    class TanksGame : public Game
    {
        inline String GetIniFile() override
        {
            return "tanks.ini";
        };

        void CoreSystemsSetup() override;
        void GameplaySystemsSetup() override;
        void WorldSetup() override;
    };
}
 
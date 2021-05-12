#pragma once

#include "core/core.h"
#include "core/game.h"
#include "core/engine.h"

using namespace dagger;

namespace roboship
{
    class Roboship : public Game
    {
        String GetIniFile() override
        {
            return "roboship.ini";
        }

        void GameplaySystemsSetup() override;
        void WorldSetup() override;
    };
}

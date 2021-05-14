#pragma once

#include "core/core.h"
#include "core/game.h"
#include "gameplay/roboship/inventorySetup.h"

using namespace dagger;
using namespace inventory;

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

        void RoboshipCreateInventory();
    };

}

void RoboshipCreateBackdrop();
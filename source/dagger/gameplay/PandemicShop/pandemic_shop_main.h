#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"

using namespace dagger;

namespace pandemic_shop
{
    
    void SetupWorld(Engine& engine_);

    class PandemicShopGame : public Game
    {
        inline String GetIniFile() override
        {
            return "pandemic.ini";
        };

        void CoreSystemsSetup(Engine& engine_) override;
        void GameplaySystemsSetup(Engine& engine_) override;
        void WorldSetup(Engine& engine_) override;
    };
}

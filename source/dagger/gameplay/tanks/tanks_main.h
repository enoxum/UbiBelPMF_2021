#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"

using namespace dagger;

namespace tanks
{
	struct Tank
	{
		Float32 speed;
		Float32 angle;
		Vector3 pos{0,0,0}; // last good position before collision
	};
	
    void SetupWorld(Engine& engine_);

    class TanksGame : public Game
    {
        inline String GetIniFile() override
        {
            return "tanks.ini";
        };

        void CoreSystemsSetup(Engine& engine_) override;
        void GameplaySystemsSetup(Engine& engine_) override;
        void WorldSetup(Engine& engine_) override;
    };
}

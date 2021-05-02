#pragma once

#include "core/core.h"
#include "core/game.h"

using namespace dagger;

namespace brawler
{
    class Brawler : public Game
    {
        String GetIniFile() override
        {
            return "brawler.ini";
        }

        void GameplaySystemsSetup() override;
        void WorldSetup() override;

        void SetCamera();
        void CreateBackdrop();
    };
}
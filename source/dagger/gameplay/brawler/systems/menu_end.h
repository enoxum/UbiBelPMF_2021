#pragma once

#include "core/core.h"
#include "core/engine.h"
#include "core/system.h"
#include "core/input/inputs.h"
#include "gameplay/brawler/menu.h"

using namespace dagger;

namespace brawler
{

    class MenuEnd : public Menu
    {
    public:
        MenuEnd();

        inline String SystemName() { return "End Menu System"; }

        void CreateMenu() override;
        void DestroyMenu() override;

        void Run() override;

    private:
        void OnReplay();
        void OnQuit();

        Bool hasWinner = false;
        String winner_name;
        Entity winner_text;
        Entity background;
    };

}

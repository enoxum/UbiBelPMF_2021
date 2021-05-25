#pragma once

#include "core/core.h"
#include "core/engine.h"
#include "core/system.h"
#include "core/input/inputs.h"
#include "gameplay/brawler/menu.h"

using namespace dagger;

namespace brawler
{

class MenuPause : public Menu
{
public:
    MenuPause();

    inline String SystemName() { return "Pause Menu System"; }

    void CreateMenu() override;
    void DestroyMenu() override;

private:
    void OnContinue();
    void OnRestart();
    void OnQuit();

    Entity background;
};

}

#pragma once

#include "core/core.h"
#include "core/engine.h"
#include "core/system.h"
#include "core/input/inputs.h"
#include "gameplay/brawler/menu.h"

using namespace dagger;

namespace brawler
{

class MenuMain : public Menu
{
public:
    MenuMain();

    inline String SystemName() { return "Main Menu System"; }

    void CreateMenu() override;
    void DestroyMenu() override;

private:
    void OnPlayLevel1();
    void OnPlayLevel2();
    void OnQuit();

    Entity background;
};

}
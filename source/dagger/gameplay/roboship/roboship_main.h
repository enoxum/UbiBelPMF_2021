#pragma once

#include "core/core.h"
#include "core/game.h"
#include "gameplay/roboship/inventorySetup.h"
#include "core/engine.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/audio.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite_render.h"
#include "core/graphics/textures.h"
#include "core/graphics/animations.h"
#include "core/graphics/gui.h"
#include "core/graphics/text.h"

#include <vector>

using namespace dagger;
using namespace inventory;

namespace roboship
{
    struct EnemyMarker { char Id; };

    class Roboship : public Game
    {

        String GetIniFile() override
        {
            return "roboship.ini";
        }

        void GameplaySystemsSetup() override;
        void WorldSetup() override;

        void TurnRobots();
        void RobotDie();
        void ShowTextPrepareFightMode();
        void ClearTextPrepareFightMode();
        void TurnOnSpaceship();
    };
}

struct RPrepareFightModeOff
{
    char _preparefightmodeoff;
};


struct RPrepareFightModeOn
{
    char _preparefightmodeon;
};

struct RChangeDirection
{
    char _placeholder;
};

struct RFightModeOn
{
    char _fightmodeon;
};

struct RFightModeOff
{
    char _fightmodeon;
};

struct REnemy {
    Entity entity;
    Sprite& sprite;
    Animator& animator;
    int numberOfTurns;
    std::vector<int> sequence;

    static REnemy* Get(Entity entity);
    static REnemy* Create(ColorRGB color_, Vector2);
    void change_direction(RChangeDirection& ev);
    std::vector<int> getSequence();
    int getNumberOfTurns();
    void fillSequence();
    void setNumberOFTurns();
};

struct RSpaceship {
    Entity entity;
    Sprite& sprite;
    Animator& animator;

    static RSpaceship* Get(Entity entity);
    static RSpaceship* Create(ColorRGB color_, Vector2);
};

extern struct REnemy;

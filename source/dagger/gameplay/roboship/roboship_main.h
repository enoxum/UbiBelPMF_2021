#pragma once

#include "core/core.h"
#include "core/game.h"
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

using namespace dagger;

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
        void ShowTextPrepareFightMode();
        void ClearTextPrepareFightMode();
    };
}

struct RPrepareFightModeOff
{
    char _fightmode;
};


struct RPrepareFightModeOn
{
    char _fightmode;
};

struct RChangeDirection
{
    char _placeholder;
};

struct REnemy {
    Entity entity;
    Sprite& sprite;
    Animator& animator;

    static REnemy* Get(Entity entity);
    static REnemy* Create(ColorRGB color_, Vector2);
    void change_direction(RChangeDirection& ev);
};

extern struct REnemy;
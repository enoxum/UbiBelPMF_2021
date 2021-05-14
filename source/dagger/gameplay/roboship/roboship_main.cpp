#include "gameplay/roboship/roboship_main.h"
#include "gameplay/roboship/selectTileController.h"
#include "gameplay/roboship/inventorySetup.h"

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



#include "tools/diagnostics.h"

using namespace dagger;
using namespace roboship;
using namespace robo_game;


void Roboship::GameplaySystemsSetup()
{
    auto& engine = Engine::Instance();
    engine.AddSystem<SelectedTileInputSystem>();

}

void RoboshipSetCamera()
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 2;
    camera->position = { 0, 0, 0 };
    camera->Update();
}

void RoboshipCreateBackdrop()
{
    auto& reg = Engine::Registry();
    auto* camera = Engine::GetDefaultResource<Camera>();


    // Create terrain 
    {
        auto back = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(back);

        AssignSprite(sprite, "EmptyWhitePixel");
        sprite.color = { 0, 0, 0, 1 };
        sprite.size = { 200, 30 };
        sprite.scale = { 10, 1 };
        sprite.position = { 0, -135, 1 };
    }


    /* Put background image */ 
    {
        auto entity = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);

        AssignSprite(sprite, "robot:BACKGROUND:background1");
        sprite.scale.x = 0.25f;
        sprite.scale.y = 0.25f;
        sprite.position.x = 0;
        sprite.position.y = 5;
        sprite.position.z = 10;
    }

    // Inventory matrix - has data - positions of the parts
    {
        auto entity = reg.create();
        auto& matInv = reg.emplace<InventoryMatrix>(entity);
    }

    Inventory* inv = new Inventory();
    inv->InventoryPositionsSetup();
    inv->SelectedTileSetup();
    inv->makeMatrix();
    inv->FillInventory();

    /*
    {
        auto ui = reg.create();
        auto& text = reg.emplace<Text>(ui);
        text.spacing = 0.6f;
        text.Set("pixel-font", "Roboship game");
    }
    */

}

void Roboship::RoboshipCreateInventory()
{

}


void Roboship::WorldSetup()
{
    RoboshipSetCamera();
    RoboshipCreateBackdrop();
    RoboshipCreateInventory();
    //Engine::Registry().emplace<CameraFollowFocus>(sndChar.entity);
}


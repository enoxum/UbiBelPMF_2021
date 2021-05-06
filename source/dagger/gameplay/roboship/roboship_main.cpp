#include "gameplay/roboship/roboship_main.h"

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

void Roboship::GameplaySystemsSetup()
{
    auto& engine = Engine::Instance();

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


    /* Put background image */ {
        auto entity = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);

        AssignSprite(sprite, "robot:BACKGROUND:background1");
        sprite.scale.x = 0.25f;
        sprite.scale.y = 0.25f;
        sprite.position.x = 0;
        sprite.position.y = 5;
        sprite.position.z = 10;
    }

    // Add inventory
    constexpr int height = 4;
    constexpr int width = 4;
    float tileSize = 30.f;// / static_cast<float>(Width);

    float zPos = 1.f;

    constexpr float Space = 0.3f;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);

            if (i == 3 && j == 0)
                AssignSprite(sprite, "robot:INVENTORY:SelectedTile");
            else
                AssignSprite(sprite, "robot:INVENTORY:Tile");

            sprite.size.x = tileSize;
            sprite.size.y = tileSize;

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = (0.5f + j + j * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
            transform.position.y = (0.5f + i + i * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;
            transform.position.z = zPos;
        }
    }

    tileSize = 40.f;

    for (int i = 0; i < 3; i++)
    {

        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);


        AssignSprite(sprite, "robot:INVENTORY:SpecialTile1");

        sprite.size.x = tileSize;
        sprite.size.y = tileSize;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = (- static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
        transform.position.y = (0.5f + i + i * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;
        transform.position.z = zPos;
    
    }

    /*
    {
        auto ui = reg.create();
        auto& text = reg.emplace<Text>(ui);
        text.spacing = 0.6f;
        text.Set("pixel-font", "Roboship game");
    }
    */
}


void Roboship::WorldSetup()
{
    RoboshipSetCamera();
    RoboshipCreateBackdrop();

    //Engine::Registry().emplace<CameraFollowFocus>(sndChar.entity);
}


#include "gameplay/roboship/inventorySetup.h"
#include "gameplay/roboship/selectTileController.h"
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
using namespace roboship;
using namespace robo_game;

void inventory::Inventory::InventoryPositionsSetup()
{
    auto& reg = Engine::Registry();

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);

            AssignSprite(sprite, "robot:INVENTORY:Tile");

            sprite.size.x = tileSize;
            sprite.size.y = tileSize;

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = (-1.0f + j + j * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
            transform.position.y = (2.5f + i + i * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;
            transform.position.z = 3.f;
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

        transform.position.x = (-1.5f - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
        transform.position.y = (2.65f + i + i * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;
        transform.position.z = zPos;


    }


}


void inventory::Inventory::SelectedTileSetup()
{
    auto& reg = Engine::Registry();

    {
        auto entity = reg.create();

        auto& sprite = reg.emplace<Sprite>(entity);

        AssignSprite(sprite, "robot:INVENTORY:SelectedTile");

        sprite.size.x = tileSize;
        sprite.size.y = tileSize;

        auto& transform = reg.emplace<Transform>(entity);

        transform.position.x = (-1.0f + 0 + 0 * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
        transform.position.y = (2.5f + 3 + 3 * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;
        transform.position.z = 1.f;

        auto& controller = reg.emplace<ControllerMapping>(entity);

    }
    
    {
        auto entity = reg.create();

        auto& sprite = reg.emplace<Sprite>(entity);

        AssignSprite(sprite, "robot:INVENTORY:SpecialTile2");

        sprite.size.x = tileSize;
        sprite.size.y = tileSize;

        auto& transform = reg.emplace<Transform>(entity);

        transform.position.x = (-1.0f + 0 + 0 * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
        transform.position.y = (2.5f + 3 + 3 * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;
        transform.position.z = 2.f;

        auto& controller = reg.emplace<ControllerMapping>(entity);
    }
}

void inventory::Inventory::FillInventory(MatrixInv m)
{
    tileSize = 30.f;

    auto& reg = Engine::Registry();

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);
            
            AssignSprite(sprite, fmt::format("robot:INVENTORY:part_{}", m[i][j]));

            sprite.size.x = 15.f;
            sprite.size.y = 15.f;

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = (-1.0f + j + j * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
            transform.position.y = (2.5f + i + i * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;
            transform.position.z = 0.f;
        }
    }
}

MatrixInv inventory::Inventory::SwapMatrix(MatrixInv m, int x, int y, int a, int b)
{
    int tmp = m[x][y];
    m[x][y] = m[a][b];
    m[a][b] = tmp;
    FillInventory(m);
    return m;
}

MatrixInv inventory::Inventory::makeMatrix()
{
    MatrixInv m;
    m.resize(4);
    for (int i = 0; i < 4; i++)
        m[i].resize(4);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m[i][j] = 1 + (rand() % 5);
    return m;
}

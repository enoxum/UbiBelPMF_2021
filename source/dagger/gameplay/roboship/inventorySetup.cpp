#include "gameplay/roboship/inventorySetup.h"
#include "gameplay/roboship/selectTileController.h"
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
#include <gameplay/roboship/roboship_player_move.h>

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

            reg.emplace<MoveWithRobot>(entity);
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

        reg.emplace<EmptySprite>(entity);
        reg.emplace<MoveWithRobot>(entity);

        
      

    }
}


void inventory::Inventory::SelectedTileSetup()
{
    auto& reg = Engine::Registry();

    auto view2 = Engine::Registry().view<Sprite, RoboshipPlayer>();
    Sprite pos;

    for (auto e : view2)
    {
        pos = Engine::Registry().get<Sprite>(e);
    }

    {
        auto entity = reg.create();

        auto& sprite = reg.emplace<Sprite>(entity);

        AssignSprite(sprite, "robot:INVENTORY:SelectedTile");
        
        sprite.size.x = tileSize;
        sprite.size.y = tileSize;

        auto& transform = reg.emplace<Transform>(entity);

        transform.position.x = (-1.0f + 0 + 0 * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize + pos.position.x;
        transform.position.y = (2.5f + 0 + 0 * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;
        transform.position.z = 1.f;

        auto& controller = reg.emplace<ControllerMapping>(entity);
        reg.emplace<Tile>(entity);
        reg.emplace<MoveWithRobot>(entity);
     
      

    }

    tileSize = 30.f;
    
    {
        auto entity = reg.create();

        auto& sprite = reg.emplace<Sprite>(entity);

        AssignSprite(sprite, "robot:INVENTORY:SelectedTile");

        sprite.size.x = tileSize;
        sprite.size.y = tileSize;

        auto& transform = reg.emplace<Transform>(entity);

        transform.position.x = (-1.0f + 0 + 0 * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize + pos.position.x;
        transform.position.y = (2.5f + 0 + 0 * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;
        transform.position.z = 4.f;
        reg.emplace<Tile>(entity);
       reg.emplace<MoveWithRobot>(entity);



    }
}

void inventory::Inventory::FillInventory()
{
    tileSize = 30.f;

    auto& reg = Engine::Registry();

    auto entityM = reg.view<InventoryMatrix>()[0];
    auto& matrix = reg.get<InventoryMatrix>(entityM);


    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);
            
            AssignSprite(sprite, fmt::format("robot:INVENTORY:part_{}", matrix.matrix[i][j]));

            sprite.size.x = 15.f;
            sprite.size.y = 15.f;

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = (-1.0f + i + i * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
            transform.position.y = (2.5f + j + j * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;
            transform.position.z = 0.f;

            reg.emplace<MoveWithRobot>(entity);

        }
    }
}

void inventory::Inventory::SwapMatrix(int x, int y, int a, int b)
{

    auto entity = Engine::Registry().view<InventoryMatrix>()[0];
    auto& matrix = Engine::Registry().get<InventoryMatrix>(entity);


    int tmp = matrix.matrix[x][y];
    matrix.matrix[x][y] = matrix.matrix[a][b];
    matrix.matrix[a][b] = tmp;
    SwapSprites(x, y, a, b);

}

void inventory::Inventory::makeMatrix()
{
    auto entity = Engine::Registry().view<InventoryMatrix>()[0];

    auto& matrix = Engine::Registry().get<InventoryMatrix>(entity);

    matrix.matrix.resize(4);
    for (int i = 0; i < 4; i++)
        matrix.matrix[i].resize(4);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            matrix.matrix[i][j] = 1 + rand() % 6;

}

void inventory::Inventory::SwapSprites(int x, int y, int a, int b)
{
    printf("swap\n");
    auto view = Engine::Registry().view<Transform, Sprite>();
    auto entityM = Engine::Registry().view<InventoryMatrix>()[0];
    auto& matrix = Engine::Registry().get<InventoryMatrix>(entityM);

    auto view2 = Engine::Registry().view<Sprite, RoboshipPlayer>();
    Sprite s;

    for (auto e : view2)
    {
        s = Engine::Registry().get<Sprite>(e);
    }

    tileSize = 30;


    float posX = (-1.0f + x + x * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize + s.position.x;
    float posY = (2.5f + y + y * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;

    float posA = (-1.0f + a + a * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize + s.position.x;
    float posB = (2.5f + b + b * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;


    for (auto entity : view)
    {
        if (Engine::Registry().has<ControllerMapping>(entity))
            continue;

        auto& t = view.get<Transform>(entity);
        auto& s = view.get<Sprite>(entity);

        if (abs(t.position.x - posX) <= 6 && t.position.y == posY && s.size.x == 15.f)
        {
            AssignSprite(s, fmt::format("robot:INVENTORY:part_{}", matrix.matrix[x][y]));
            s.size.x = 15.f;
            s.size.y = 15.f;
        }

        if (abs(t.position.x - posA) <= 6 && t.position.y == posB && s.size.x == 15.f)
        {
            AssignSprite(s, fmt::format("robot:INVENTORY:part_{}", matrix.matrix[a][b]));
            s.size.x = 15.f;
            s.size.y = 15.f;
        }
    }
}

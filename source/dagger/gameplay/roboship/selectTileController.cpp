#include "selectTileController.h"

#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"
#include "gameplay/roboship/fightEnemy.h"
#include "gameplay/roboship/roboship_main.h"
#include <gameplay/roboship/roboship_player_move.h>

using namespace dagger;
using namespace robo_game;
using namespace roboship;
using namespace fightEnemy;

void SelectedTileInputSystem::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&SelectedTileInputSystem::OnKeyboardEvent>(this);
}

void SelectedTileInputSystem::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&SelectedTileInputSystem::OnKeyboardEvent>(this);
}

void SelectedTileInputSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    Engine::Dispatcher().sink<RFightModeOn>().connect<&SelectedTileInputSystem::setFightModeOn>(this);
    Engine::Dispatcher().sink<RFightModeOff>().connect<&SelectedTileInputSystem::setFightModeOff>(this);

    auto entity2 = Engine::Registry().create();
     
    auto info = Engine::Registry().view<RFightModeOn>()[0];

    auto& fightMode = Engine::Registry().get<RFightModeOn>(info);

    auto comb = fightMode.combination;

    if (fightMode.found)
        fightModeOn = false;
    
    Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
    {
        tileSize = 80.f;
        if (!fightModeOn)
            return;

        if (kEvent_.key == ctrl_.leftKey && kEvent_.action == EDaggerInputState::Pressed && ctrl_.input.x > 0)
        {
            ctrl_.input.x -= 1;
        }
        else if (kEvent_.key == ctrl_.rightKey && kEvent_.action == EDaggerInputState::Pressed && ctrl_.input.x < 3)
        {
            ctrl_.input.x += 1;
        }
        else if (kEvent_.key == ctrl_.upKey && kEvent_.action == EDaggerInputState::Pressed && ctrl_.input.y < 3)
        {
            ctrl_.input.y += 1;
        }
        else if (kEvent_.key == ctrl_.downKey && kEvent_.action == EDaggerInputState::Pressed && ctrl_.input.y > 0)
        {
            ctrl_.input.y -= 1;
        }
        else if (kEvent_.key == ctrl_.spaceKey && kEvent_.action == EDaggerInputState::Pressed)
        {
            if (fightMode.moves == 0) {
                Engine::Dispatcher().trigger<RGameOver>();
                return;
            }
            /*
            if (moveFirst)
            {
                moveFirst = false;
                moveSecond = true;
                x = ctrl_.input.x;
                y = ctrl_.input.y;
            }
            else
            {
                moveFirst = true;
                moveSecond = false;
                swapX = ctrl_.input.x;
                swapY = ctrl_.input.y;

                Inventory* inv = new Inventory();
                inv->SwapMatrix(x, y, swapX, swapY);
            }

            */

            if (!swap)
            {
                auto entity = Engine::Registry().view<ControllerMapping>()[0];
                auto& s = Engine::Registry().get<Sprite>(entity);

                AssignSprite(s, "robot:INVENTORY:SpecialTile2");
                s.size.x = tileSize;
                s.size.y = tileSize;

                x = ctrl_.input.x;
                y = ctrl_.input.y;

                auto view = Engine::Registry().view<Tile, Sprite, Transform>();

                for (auto entity2 : view)
                {
                    if (Engine::Registry().has<ControllerMapping>(entity2))
                        continue;

                    auto& s2 = Engine::Registry().get<Sprite>(entity2);
                    s2.size.x = tileSize;
                    s2.size.y = tileSize;

                    auto& t = Engine::Registry().get<Transform>(entity2);
                    t.position.z = 2.f;

                    auto& controller = Engine::Registry().emplace<ControllerMapping>(entity2);

                }
                
                Engine::Registry().remove<ControllerMapping>(entity);
                swap = true;
                MarkNeighbors(x, y);
            }
            else
            {
                if ((ctrl_.input.x == (x) && ctrl_.input.y == (y - 1)) || (ctrl_.input.x == (x) && ctrl_.input.y == (y + 1)) || (ctrl_.input.x == (x - 1) && ctrl_.input.y == (y)) || (ctrl_.input.x == (x + 1) && ctrl_.input.y == (y)))
                {

                    swapX = ctrl_.input.x;
                    swapY = ctrl_.input.y;

                    tileSize = 60.f;

                    Inventory* inv = new Inventory();
                    inv->SwapMatrix(x, y, swapX, swapY);

                    auto view = Engine::Registry().view<Tile>();

                    for (auto entity : view)
                    {
                        if (Engine::Registry().has<ControllerMapping>(entity))
                            continue;

                        auto& s = Engine::Registry().get<Sprite>(entity);

                        AssignSprite(s, "robot:INVENTORY:SelectedTile");
                        s.size.x = tileSize/2;
                        s.size.y = tileSize/2;

                        auto& t = Engine::Registry().get<Transform>(entity);
                        t.position.z = 4.f;
                    }

                    swap = false;

                    
                    UnmarkNeighbors();
                    for (int i = 0; i < comb.size(); i++)
                    findCombination(comb);
                    fightMode.moves--;
                }
            }
        }
        else return;
    });
}

void SelectedTileInputSystem::MarkNeighbors(int x, int y)
{
    auto view = Engine::Registry().view<Transform, Sprite>();
    auto view2 = Engine::Registry().view<Sprite, RoboshipPlayer>();
    Sprite pos;
    tileSize = 60.f;

    for (auto e : view2)
    {
        pos = Engine::Registry().get<Sprite>(e);
    }



    for (auto entity : view)
    {
        if (Engine::Registry().has<ControllerMapping>(entity))
            continue;

        if (x - 1 >= 0)
        {
            float posX = (-1.0f + x - 1 + (x - 1) * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize + pos.position.x;
            float posY = (2.5f + y + y * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;


            auto& t = view.get<Transform>(entity);
            auto& s = view.get<Sprite>(entity);


            if (abs(t.position.x - posX) <= 10 && abs(t.position.y - posY) <= 10 && s.size.x == tileSize)
            {
                AssignSprite(s, "robot:INVENTORY:SpecialTile1");
                s.size.x = tileSize;
                s.size.y = tileSize;
                Engine::Registry().emplace<MarkedTile>(entity);
            }
        }
        if (x + 1 < 4)
        {
            float posX = (-1.0f + x + 1 + (x + 1) * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize + pos.position.x;
            float posY = (2.5f + y + y * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;


            auto& t = view.get<Transform>(entity);
            auto& s = view.get<Sprite>(entity);

            if (abs(t.position.x - posX) <= 10 && abs(t.position.y - posY) <= 10 && s.size.x == tileSize)
            {
                AssignSprite(s, "robot:INVENTORY:SpecialTile1");
                s.size.x = tileSize;
                s.size.y = tileSize;
                Engine::Registry().emplace<MarkedTile>(entity);
            }
        }

        if (y - 1 >= 0)
        {
            float posX = (-1.0f + x + x * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize + pos.position.x;
            float posY = (2.5f + y - 1 + (y - 1) * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;


            auto& t = view.get<Transform>(entity);
            auto& s = view.get<Sprite>(entity);

            if (abs(t.position.x - posX) <= 10 && abs(t.position.y - posY) <= 10 && s.size.x == tileSize)
            {
                AssignSprite(s, "robot:INVENTORY:SpecialTile1");
                s.size.x = tileSize;
                s.size.y = tileSize;
                Engine::Registry().emplace<MarkedTile>(entity);
            }
        }

        if (y + 1 < 4)
        {
            float posX = (-1.0f + x + x * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize + pos.position.x;
            float posY = (2.5f + y + 1 + (y + 1) * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;


            auto& t = view.get<Transform>(entity);
            auto& s = view.get<Sprite>(entity);

            if (abs(t.position.x - posX) <= 10 && abs(t.position.y - posY) <= 10 && s.size.x == tileSize)
            {
                AssignSprite(s, "robot:INVENTORY:SpecialTile1");
                s.size.x = tileSize;
                s.size.y = tileSize;
                Engine::Registry().emplace<MarkedTile>(entity);
            }
        }
    }
}

void SelectedTileInputSystem::UnmarkNeighbors()
{
    auto view = Engine::Registry().view<Sprite, MarkedTile>();
    tileSize = 60.f;

    for (auto entity : view)
    {
        auto& s = view.get<Sprite>(entity);
        AssignSprite(s, "robot:INVENTORY:Tile");                
        s.size.x = tileSize;
        s.size.y = tileSize;
        Engine::Registry().remove<MarkedTile>(entity);
    }
}



void SelectedTileInputSystem::setFightModeOn()
{
    fightModeOn = true;
}

void SelectedTileInputSystem::setFightModeOff()
{
    fightModeOn = false;
}

void SelectedTileInputSystem::Run()
{
    tileSize = 60.f;
    auto view = Engine::Registry().view<Transform, ControllerMapping>();
    auto view2 = Engine::Registry().view<Sprite, RoboshipPlayer>();
    Sprite s;

    for (auto e : view2)
    {
        s = Engine::Registry().get<Sprite>(e);
    }
    

        for (auto entity : view)
        {

            auto& t = view.get<Transform>(entity);

            auto& ctrl = view.get<ControllerMapping>(entity);
            

            t.position.x = (-1.0f + ctrl.input.x + ctrl.input.x * Space - static_cast<float>(4 * (1 + Space)) / 2.f) * tileSize + s.position.x;
            t.position.y = (2.5f + ctrl.input.y + ctrl.input.y * Space - static_cast<float>(4 * (1 + Space)) / 2.f) * tileSize;
        }
    
}
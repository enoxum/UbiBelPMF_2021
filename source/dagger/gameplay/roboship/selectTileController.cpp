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
   Engine::Dispatcher().sink<setfightmodeon>().connect<&SelectedTileInputSystem::setFightModeOn>(this);
    

    auto entity2 = Engine::Registry().create();
     
    auto info = Engine::Registry().view<RFightModeOn>()[0];

    auto& fightMode = Engine::Registry().get<RFightModeOn>(info);

    auto comb = fightMode.combination;

    if (fightMode.found)
        fightModeOn = false;
    

    Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
    {

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
            if (fightMode.moves== 0)
                return;
            /*
            if (moveFirst)
            {
                moveFirst = false;
                moveSecond = true;
                x = ctrl_.input.x;
                y = ctrl_.input.y;
                printf("%d %d", x, y);
            }
            else
            {
                moveFirst = true;
                moveSecond = false;
                swapX = ctrl_.input.x;
                swapY = ctrl_.input.y;
                printf("%d %d", swapX, swapY);

                Inventory* inv = new Inventory();
                inv->SwapMatrix(x, y, swapX, swapY);
                printf("nisam");
            }

            */

            if (!swap)
            {
                auto entity = Engine::Registry().view<ControllerMapping>()[0];
                auto& s = Engine::Registry().get<Sprite>(entity);

                AssignSprite(s, "robot:INVENTORY:SpecialTile2");
                s.size.x = 80.f;
                s.size.y = 80.f;

                x = ctrl_.input.x;
                y = ctrl_.input.y;

                auto view = Engine::Registry().view<Tile>();

                for (auto entity2 : view)
                {
                    if (Engine::Registry().has<ControllerMapping>(entity2))
                        continue;

                    auto& s2 = Engine::Registry().get<Sprite>(entity2);
                    s2.size.x = 80.f;
                    s2.size.y = 80.f;

                    auto& t = Engine::Registry().get<Transform>(entity2);
                    t.position.z = 1.f;

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

                    Inventory* inv = new Inventory();
                    inv->SwapMatrix(x, y, swapX, swapY);

                    auto view = Engine::Registry().view<Tile>();

                    for (auto entity : view)
                    {
                        if (Engine::Registry().has<ControllerMapping>(entity))
                            continue;

                        printf("Ovde se postavlja selected\n");

                        auto& s = Engine::Registry().get<Sprite>(entity);

                        AssignSprite(s, "robot:INVENTORY:SelectedTile");
                        s.size.x = 30.f;
                        s.size.y = 30.f;

                        auto& t = Engine::Registry().get<Transform>(entity);
                        t.position.z = 4.f;
                    }

                    swap = false;

                    
                    UnmarkNeighbors();
                    for (int i = 0; i < comb.size(); i++)
                    printf("%d", comb[i]);
                    printf("\n;");
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
            float posX = (-1.0f + x - 1 + (x - 1) * Space - static_cast<float>(width * (1 + Space)) / 2.f) * 60.f + pos.position.x;
            float posY = (2.5f + y + y * Space - static_cast<float>(height * (1 + Space)) / 2.f) * 60.f;


            auto& t = view.get<Transform>(entity);
            auto& s = view.get<Sprite>(entity);


            if (abs(t.position.x - posX) <= 10 && abs(t.position.y - posY) <= 10 && s.size.x == 60.f)
            {
                AssignSprite(s, "robot:INVENTORY:SpecialTile1");
                s.size.x = 60.f;
                s.size.y = 60.f;
                Engine::Registry().emplace<MarkedTile>(entity);
            }
        }
        if (x + 1 < 4)
        {
            float posX = (-1.0f + x + 1 + (x + 1) * Space - static_cast<float>(width * (1 + Space)) / 2.f) * 60.f + pos.position.x;
            float posY = (2.5f + y + y * Space - static_cast<float>(height * (1 + Space)) / 2.f) * 60.f;


            auto& t = view.get<Transform>(entity);
            auto& s = view.get<Sprite>(entity);

            if (abs(t.position.x - posX) <= 10 && abs(t.position.y - posY) <= 10 && s.size.x == 60.f)
            {
                AssignSprite(s, "robot:INVENTORY:SpecialTile1");
                s.size.x = 60.f;
                s.size.y = 60.f;
                Engine::Registry().emplace<MarkedTile>(entity);
            }
        }

        if (y - 1 >= 0)
        {
            float posX = (-1.0f + x + x * Space - static_cast<float>(width * (1 + Space)) / 2.f) * 60.f + pos.position.x;
            float posY = (2.5f + y - 1 + (y - 1) * Space - static_cast<float>(height * (1 + Space)) / 2.f) * 60.f;


            auto& t = view.get<Transform>(entity);
            auto& s = view.get<Sprite>(entity);

            if (abs(t.position.x - posX) <= 10 && abs(t.position.y - posY) <= 10 && s.size.x == 60.f)
            {
                AssignSprite(s, "robot:INVENTORY:SpecialTile1");
                s.size.x = 60.f;
                s.size.y = 60.f;
                Engine::Registry().emplace<MarkedTile>(entity);
            }
        }

        if (y + 1 < 4)
        {
            float posX = (-1.0f + x + x * Space - static_cast<float>(width * (1 + Space)) / 2.f) * 60.f + pos.position.x;
            float posY = (2.5f + y + 1 + (y + 1) * Space - static_cast<float>(height * (1 + Space)) / 2.f) * 60.f;


            auto& t = view.get<Transform>(entity);
            auto& s = view.get<Sprite>(entity);

            if (abs(t.position.x - posX) <= 10 && abs(t.position.y - posY) <= 10 && s.size.x == 60.f)
            {
                AssignSprite(s, "robot:INVENTORY:SpecialTile1");
                s.size.x = 60.f;
                s.size.y = 60.f;
                Engine::Registry().emplace<MarkedTile>(entity);
            }
        }
    }
}

void SelectedTileInputSystem::UnmarkNeighbors()
{
    auto view = Engine::Registry().view<Sprite, MarkedTile>();

    for (auto entity : view)
    {
        auto& s = view.get<Sprite>(entity);
        AssignSprite(s, "robot:INVENTORY:Tile");                
        s.size.x = 60.f;
        s.size.y = 60.f;
        Engine::Registry().remove<MarkedTile>(entity);
    }
}



void SelectedTileInputSystem::setFightModeOn()
{
    fightModeOn = true;
}

void SelectedTileInputSystem::Run()
{

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
            

            t.position.x = (-1.0f + ctrl.input.x + ctrl.input.x * 0.3f - static_cast<float>(4 * (1 + 0.3f)) / 2.f) * 60.f + s.position.x;
            t.position.y = (2.5f + ctrl.input.y + ctrl.input.y * 0.3f - static_cast<float>(4 * (1 + 0.3f)) / 2.f) * 60.f;
        }
    
}
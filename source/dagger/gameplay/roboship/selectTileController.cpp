#include "selectTileController.h"

#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"
#include "gameplay/roboship/fightEnemy.h"
#include "gameplay/roboship/roboship_main.h"

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
    Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
        {
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
                    s.size.x = 40;
                    s.size.y = 40;

                    x = ctrl_.input.x;
                    y = ctrl_.input.y;

                    auto view = Engine::Registry().view<Tile>();

                    for (auto entity2 : view)
                    {
                        if (Engine::Registry().has<ControllerMapping>(entity2))
                            continue;

                        auto& s2 = Engine::Registry().get<Sprite>(entity2);
                        s2.size.x = 40;
                        s2.size.y = 40;

                        auto& t = Engine::Registry().get<Transform>(entity2);

                        t.position.z = 1.f;

                        auto& controller = Engine::Registry().emplace<ControllerMapping>(entity2);

                    }

                    Engine::Registry().remove<ControllerMapping>(entity);
                    swap = true;
                }
                else
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

                        auto& s = Engine::Registry().get<Sprite>(entity);

                        AssignSprite(s, "robot:INVENTORY:SelectedTile");                        
                        s.size.x = 30;
                        s.size.y = 30;

                        auto& t = Engine::Registry().get<Transform>(entity);
                        t.position.z = 4.f;
                    }

                    swap = false;

                    bool found;
                    found = findCombination({ 1, 1, 1});
                }
            }

        });
}

void SelectedTileInputSystem::Run()
{

    auto view = Engine::Registry().view<Transform, ControllerMapping>();
    for (auto entity : view)
    {

        auto& t = view.get<Transform>(entity);

        auto& ctrl = view.get<ControllerMapping>(entity);

        t.position.x = (-1.0f + ctrl.input.x + ctrl.input.x * 0.3f - static_cast<float>(4 * (1 + 0.3f)) / 2.f) * 30.f;
        t.position.y = (2.5f + ctrl.input.y + ctrl.input.y * 0.3f - static_cast<float>(4 * (1 + 0.3f)) / 2.f) * 30.f;
    }
}
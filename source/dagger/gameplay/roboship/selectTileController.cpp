#include "selectTileController.h"

#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"

using namespace dagger;
using namespace robo_game;

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

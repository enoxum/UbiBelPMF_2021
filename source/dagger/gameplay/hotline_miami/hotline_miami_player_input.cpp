#include "hotline_miami_player_input.h"

#include "core/engine.h"
#include "core/game/transforms.h"

using namespace dagger;
using namespace hotline_miami;


Float32 HotlineMiamiPlayerInputSystem::s_PlayerSpeed = 30.f;

void HotlineMiamiPlayerInputSystem::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&HotlineMiamiPlayerInputSystem::OnKeyboardEvent>(this);
}

void HotlineMiamiPlayerInputSystem::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&HotlineMiamiPlayerInputSystem::OnKeyboardEvent>(this);
}

void HotlineMiamiPlayerInputSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
        {
            // top and down
            if (kEvent_.key == ctrl_.up_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
            {
                ctrl_.input.y = 1;
            }
            else if (kEvent_.key == ctrl_.up_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.y > 0)
            {
                ctrl_.input.y = 0;
            }
            else if (kEvent_.key == ctrl_.down_key && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
            {
                ctrl_.input.y = -1;
            }
            else if (kEvent_.key == ctrl_.down_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.y < 0)
            {
                ctrl_.input.y = 0;
            }
            // left and right
            else if (kEvent_.key == ctrl_.left_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
            {
                ctrl_.input.x = -1;
            }
            else if (kEvent_.key == ctrl_.left_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x < 0)
            {
                ctrl_.input.x = 0;
            }
            else if (kEvent_.key == ctrl_.right_key && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
            {
                ctrl_.input.x = 1;
            }
            else if (kEvent_.key == ctrl_.right_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x > 0)
            {
                ctrl_.input.x = 0;
            }
        });
}

void HotlineMiamiPlayerInputSystem::Run()
{
    auto view = Engine::Registry().view<Transform, ControllerMapping>();
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& ctrl = view.get<ControllerMapping>(entity);

        t.position.y += ctrl.input.y * s_PlayerSpeed * Engine::DeltaTime();
        t.position.x += ctrl.input.x * s_PlayerSpeed * Engine::DeltaTime();
    }
}

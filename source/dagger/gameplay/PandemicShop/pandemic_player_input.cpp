#include "pandemic_player_input.h"

#include "core/engine.h"
#include "core/game/transforms.h"

using namespace dagger;
using namespace pandemic_shop;

Float32 PandemicShopPlayerInputSystem::s_BoarderDown = -20;
Float32 PandemicShopPlayerInputSystem::s_BoarderUp = 20;
Float32 PandemicShopPlayerInputSystem::s_BoarderRight = 20;
Float32 PandemicShopPlayerInputSystem::s_BoarderLeft = -20;

Float32 PandemicShopPlayerInputSystem::s_PlayerSpeed = 1.f;

void PandemicShopPlayerInputSystem::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&PandemicShopPlayerInputSystem::OnKeyboardEvent>(this);
}

void PandemicShopPlayerInputSystem::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&PandemicShopPlayerInputSystem::OnKeyboardEvent>(this);
}

void PandemicShopPlayerInputSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
    {
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

        if (kEvent_.key == ctrl_.right_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
        {
            ctrl_.input.x = 1;
        }
        else if (kEvent_.key == ctrl_.right_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x > 0)
        {
            ctrl_.input.x = 0;
        }
        else if (kEvent_.key == ctrl_.left_key && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
        {
            ctrl_.input.x = -1;
        }
        else if (kEvent_.key == ctrl_.left_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x < 0)
        {
            ctrl_.input.x = 0;
        }
    });
}

void PandemicShopPlayerInputSystem::Run()
{
    auto view = Engine::Registry().view<Transform, ControllerMapping>();
    for (auto entity : view)
    {
        auto &t = view.get<Transform>(entity);
        auto &ctrl = view.get<ControllerMapping>(entity);

        t.position.y += ctrl.input.y * s_PlayerSpeed * Engine::DeltaTime();
        t.position.x += ctrl.input.x * s_PlayerSpeed * Engine::DeltaTime();

        if (t.position.y > s_BoarderUp)
        {
            t.position.y = s_BoarderUp;
        }

        if (t.position.y < s_BoarderDown)
        {
            t.position.y = s_BoarderDown;
        }

        if (t.position.x > s_BoarderRight)
        {
            t.position.x = s_BoarderRight;
        }

        if (t.position.x < s_BoarderLeft)
        {
            t.position.x = s_BoarderLeft;
        }
    }
}

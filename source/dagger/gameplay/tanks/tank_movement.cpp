#include "tank_movement.h"

#include "core/engine.h"
#include "core/game/transforms.h"

using namespace dagger;
using namespace tanks;

Float32 TankMovement::s_PlayerSpeed = 60.f;

void TankMovement::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&TankMovement::OnKeyboardEvent>(this);
}

void TankMovement::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&TankMovement::OnKeyboardEvent>(this);
}

void TankMovement::OnKeyboardEvent(KeyboardEvent kEvent_)
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
    });
}

void TankMovement::Run()
{
    auto view = Engine::Registry().view<Transform, ControllerMapping>();
    for (auto entity : view)
    {
        auto &t = view.get<Transform>(entity);
        auto &ctrl = view.get<ControllerMapping>(entity);

        t.position.y += ctrl.input.y * s_PlayerSpeed * Engine::DeltaTime();
    }
}

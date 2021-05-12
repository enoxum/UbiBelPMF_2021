#include "hotline_miami_player_input.h"
#include "hotline_miami_player.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

using namespace dagger;
using namespace hotline_miami;


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
            // moving
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

            // looking
            if (kEvent_.key == ctrl_.look_up_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
            {
                ctrl_.look_direction = look_top;
            }
            else if (kEvent_.key == ctrl_.look_down_key && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
            {
                ctrl_.look_direction = look_down;
            }
            else if (kEvent_.key == ctrl_.look_left_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
            {
                ctrl_.look_direction  = look_left;
            }
            else if (kEvent_.key == ctrl_.look_right_key && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
            {
                ctrl_.look_direction = look_right;
            }
        });
}

void HotlineMiamiPlayerInputSystem::Run()
{
    auto view = Engine::Registry().view<Transform, ControllerMapping, HotlineMiamiPlayer, Sprite>();
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& ctrl = view.get<ControllerMapping>(entity);
        auto& player = view.get<HotlineMiamiPlayer>(entity);
        auto& sprite = view.get<Sprite>(entity);
    
        t.position.y += ctrl.input.y * player.s_PlayerSpeed * Engine::DeltaTime();
        t.position.x += ctrl.input.x * player.s_PlayerSpeed * Engine::DeltaTime();

        sprite.rotation = ctrl.look_direction * 90.f;
       
    }   
}

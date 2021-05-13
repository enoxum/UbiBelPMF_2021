#pragma once
#include <imgui/imgui.h>
#include "atonement_start_menu.h"
#include "core/core.h"
#include "core/engine.h"
#include "gameplay/atonement/atonement_main.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"


using namespace dagger;
using namespace atonement;

String AtonementStartMenu::SystemName(){return "AtonementStartMenu";}

void AtonementStartMenu::SpinUp(){
Engine::Dispatcher().sink<KeyboardEvent>().template connect<&AtonementStartMenu::OnKeyboardEvent>(this);
}

void AtonementStartMenu::Run(){
 auto view = Engine::Registry().view<Transform, SelectionMapping>();
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& ctrl = view.get<SelectionMapping>(entity);

        t.position.x = (-1.0f + ctrl.input.x + ctrl.input.x * 0.3f - static_cast<float>(4 * (1 + 0.3f)) / 2.f) * 30.f;
        t.position.y = (2.5f + ctrl.input.y + ctrl.input.y * 0.3f - static_cast<float>(4 * (1 + 0.3f)) / 2.f) * 30.f;
    }

}

void AtonementStartMenu::WindDown(){
Engine::Dispatcher().sink<KeyboardEvent>().template disconnect<&AtonementStartMenu::OnKeyboardEvent>(this);
}

void AtonementStartMenu::OnKeyboardEvent(KeyboardEvent kEvent_){
     Engine::Registry().view<SelectionMapping>().each([&](SelectionMapping& ctrl_)
        {

            if (kEvent_.key == ctrl_.upKey && kEvent_.action == EDaggerInputState::Pressed && ctrl_.input.y < 5)
            {
                ctrl_.input.y += 5;
            }
            else if (kEvent_.key == ctrl_.downKey && kEvent_.action == EDaggerInputState::Pressed && ctrl_.input.y > 0)
            {
                ctrl_.input.y -= 5;
            }

        });
}
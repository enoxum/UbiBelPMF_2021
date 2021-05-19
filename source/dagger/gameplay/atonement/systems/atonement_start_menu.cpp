#pragma once
#include <imgui/imgui.h>
#include "atonement_start_menu.h"
#include "core/core.h"
#include "core/engine.h"
#include "gameplay/atonement/atonement_main.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"
#include "atonement_pause_system.h"


using namespace dagger;
using namespace atonement;

String AtonementStartMenu::SystemName(){return "AtonementStartMenu";}

void AtonementStartMenu::SpinUp(){
    Engine::Dispatcher().sink<KeyboardEvent>().template connect<&AtonementStartMenu::OnKeyboardEvent>(this);
    AtonementStartMenu::BuildMenu();
}

void AtonementStartMenu::Run(){
    Select();
}

void AtonementStartMenu::BuildMenu(){

    Engine::ToggleSystemsPause(true);
    AtonementPauseSystem::setPausedESC(true);
    auto& reg = Engine::Registry();
    constexpr int width = 4;
    constexpr int height = 2;
    float tileSize = 300.f;
    constexpr float Space = 0.3f;

    auto entity = reg.create();
    auto& sprite = reg.emplace<Sprite>(entity);
    AssignSprite(sprite, "MenuTextures:selected");

    sprite.size.x = tileSize;
    sprite.size.y = tileSize/2;

    auto& transform = reg.emplace<Transform>(entity);

        transform.position.x = (2.0f) * tileSize;
        transform.position.y = tileSize/2;
        transform.position.z = 1.f;

    auto& controller = reg.emplace<SelectionMapping>(entity);
    auto& onscreen = reg.emplace<OnScreenToggle>(entity);
    
    
    auto entity2 = reg.create();
    auto& sprite2 = reg.emplace<Sprite>(entity2);
    AssignSprite(sprite2, "MenuTextures:exit");
        
    auto& transform2 = reg.emplace<Transform>(entity2);

    transform2.position.x = (1.94f + Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
    transform2.position.y = (1.277f - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize/2;
    transform2.position.z = 2.f;

    auto& onscreen2 = reg.emplace<OnScreenToggle>(entity2);

    auto entity3 = reg.create();
    auto& sprite3 = reg.emplace<Sprite>(entity3);
    AssignSprite(sprite3, "MenuTextures:start");
        
    auto& transform3 = reg.emplace<Transform>(entity3);

    transform3.position.x = (1.94f + Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
    transform3.position.y = (1.277f + 1 + 1 * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize/2;
    transform3.position.z = 2.f;

    auto& onscreen3 = reg.emplace<OnScreenToggle>(entity3);
    

    auto entity4 = reg.create();
    auto& sprite4 = reg.emplace<Sprite>(entity4);
    AssignSprite(sprite4, "MenuTextures:start_menu_background");

    auto& transform4 = reg.emplace<Transform>(entity4);

        transform4.position.x = 0.3f;
        transform4.position.y = 2.0f;
        transform4.position.z = 3.f;

    auto& onscreen4 = reg.emplace<OnScreenToggle>(entity4);
}

void AtonementStartMenu::Select(){

 auto view = Engine::Registry().view<Transform, SelectionMapping>();
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& ctrl = view.get<SelectionMapping>(entity);

        t.position.x = (-1.0f + ctrl.input.x + ctrl.input.x * 0.3f - static_cast<float>(4 * (1 + 0.3f)) / 2.f) * 30.f;
        t.position.y = (2.5f + ctrl.input.y + ctrl.input.y * 0.3f - static_cast<float>(4 * (1 + 0.3f)) / 2.f) * 30.f;
    }

}

void AtonementStartMenu::RemoveFromScreen(){
    AtonementPauseSystem::setPausedESC(false);
    auto view = Engine::Registry().view<Transform, OnScreenToggle>();
    for (auto entity : view){
        auto& t = view.get<Transform>(entity);
        auto& ctrl = view.get<OnScreenToggle>(entity); 

        t.position.z = -1;
    }
}

void AtonementStartMenu::WindDown(){
Engine::Dispatcher().sink<KeyboardEvent>().template disconnect<&AtonementStartMenu::OnKeyboardEvent>(this);
}

void AtonementStartMenu::OnKeyboardEvent(KeyboardEvent kEvent_){
     Engine::Registry().view<SelectionMapping>().each([&](SelectionMapping& ctrl_)
        {

            if (kEvent_.key == ctrl_.downKey && kEvent_.action == EDaggerInputState::Pressed && ctrl_.input.y > 0)
            {
                ctrl_.input.y -= 5;
            }
            else if (kEvent_.key == ctrl_.upKey && kEvent_.action == EDaggerInputState::Pressed && ctrl_.input.y < 5)
            {
                ctrl_.input.y += 5;
            }
            else if (kEvent_.key == ctrl_.enterKey && kEvent_.action == EDaggerInputState::Pressed && ctrl_.input.y > 0)
            {
                RemoveFromScreen();
                Engine::ToggleSystemsPause(false);
            }
            else if (kEvent_.key == ctrl_.enterKey && kEvent_.action == EDaggerInputState::Pressed && ctrl_.input.y < 5)
            {
                 Engine::Dispatcher().trigger<Exit>();                 
            }
            else if (kEvent_.key == ctrl_.leftKey && kEvent_.action == EDaggerInputState::Pressed)
            {
                AtonementStartMenu::BuildMenu();
            }
        }
        );
        
}
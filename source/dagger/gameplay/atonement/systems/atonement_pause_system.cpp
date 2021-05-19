#pragma once

#include "gameplay/atonement/systems/atonement_pause_system.h"
#include "core/engine.h"
#include "core/system.h"
#include <iostream>

using namespace dagger;

bool AtonementPauseSystem::pausedESC = true;

AtonementPauseSystem::AtonementPauseSystem(){
    canBePaused = false;
};
void AtonementPauseSystem::SpinUp()
{
    pausedESC = true;
    Engine::Dispatcher().sink<KeyboardEvent>().template connect<&AtonementPauseSystem::OnKeyboardEvent>(this);
}

void AtonementPauseSystem::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().template disconnect<&AtonementPauseSystem::OnKeyboardEvent>(this);
}
void AtonementPauseSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
  if (kEvent_.key == EDaggerKeyboard::KeyEscape && kEvent_.action == EDaggerInputState::Released && pausedESC == false)
        {
            std::cout<<"Pressed ESC\n";
            if (Engine::s_IsPaused == false){
              Engine::Instance().ToggleSystemsPause(true);  
            }
            else{
                Engine::Instance().ToggleSystemsPause(false);
            }
            
        }
}

void AtonementPauseSystem::setPausedESC(bool value)
{
    pausedESC = value;
}
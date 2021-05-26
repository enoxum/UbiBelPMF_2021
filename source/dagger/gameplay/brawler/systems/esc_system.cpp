#include "esc_system.h"

#include "gameplay/brawler/systems/menu_pause.h"
#include "gameplay/brawler/systems/menu_main.h"
#include "gameplay/brawler/systems/menu_end.h"
#include "gameplay/brawler/systems/intro_system.h"

using namespace dagger;
using namespace brawler;

EscSystem::EscSystem()
{

}

void EscSystem::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&EscSystem::OnKeyboardEvent>(this);
}

void EscSystem::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&EscSystem::OnKeyboardEvent>(this);
}

void EscSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    if (kEvent_.key == EDaggerKeyboard::KeyEscape && kEvent_.action == EDaggerInputState::Released)
    {
        if(!Engine::GetDefaultResource<MenuMain>()->isVisible() && 
            !Engine::GetDefaultResource<MenuEnd>()->isVisible() &&
            Engine::GetDefaultResource<IntroSystem>()->isDone()
        )
        Engine::GetDefaultResource<MenuPause>()->Toggle();
    }
}

void EscSystem::Run()
{

}
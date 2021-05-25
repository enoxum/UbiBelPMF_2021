#include "esc_system.h"

#include "gameplay/brawler/systems/menu_pause.h"

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
        Engine::GetDefaultResource<MenuPause>()->Toggle();
    }
}

void EscSystem::Run()
{

}
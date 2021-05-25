#include "menu.h"

#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "core/graphics/text.h"

using namespace dagger;
using namespace brawler;

Menu::Menu(Bool isShown_, Bool shouldPause_)
    : buttons(), isShown(isShown_), shouldPause(shouldPause_), selected(0)
{
    
}

void Menu::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&Menu::OnKeyboardEvent>(this);
    if (isShown)
    {
        CreateMenu();
        if(shouldPause)
            Engine::ToggleSystemsPause(true);
    }
}

void Menu::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&Menu::OnKeyboardEvent>(this);
}

void Menu::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    if (!isShown)
        return;

    auto prevSelected = selected;
    if (kEvent_.key == EDaggerKeyboard::KeyDown && kEvent_.action == EDaggerInputState::Released)
    {
        selected = std::min<int>(selected + 1, buttons.size()-1);
    }
    else if (kEvent_.key == EDaggerKeyboard::KeyUp && kEvent_.action == EDaggerInputState::Released)
    {
        selected = std::max(selected - 1, 0);
    }

    if (kEvent_.key == EDaggerKeyboard::KeyEnter && kEvent_.action == EDaggerInputState::Released)
    {
        if (buttons[selected].onSelected)
            buttons[selected].onSelected();
        else
            Logger::error("Button not bound to function");
    }

    if (prevSelected == selected)
        return;

    auto& reg = Engine::Registry();

    auto& prevButton = buttons[prevSelected];
    auto& pbSprite = reg.get<Sprite>(prevButton.background);
    AssignSprite(pbSprite, prevButton.textureDefault);

    auto& button = buttons[selected];
    auto& bSprite = reg.get<Sprite>(button.background);
    AssignSprite(bSprite, button.textureSelected);
}

Button& Menu::AddButton(Float32 x, Float32 y, String text, String textureDefault, String textureSelected)
{
    auto& reg = Engine::Registry();

    auto background = reg.create();
    auto& backgroundTransform = reg.emplace<Transform>(background);
    backgroundTransform.position = { x, y, 0.0f };
    auto& backgroundSprite = reg.emplace<Sprite>(background);
    if (buttons.empty())
        AssignSprite(backgroundSprite, textureSelected);
    else
        AssignSprite(backgroundSprite, textureDefault);
    backgroundSprite.UseAsUI();
    backgroundSprite.position = { x, y, 1.0f };
    backgroundSprite.scale = { 5, 5 };

    auto foreground = reg.create();
    auto& foregroundText = reg.emplace<Text>(foreground);

    foregroundText.letterSize = { 37.0f, 47.0f };
    foregroundText.spacing = 0.7;
    foregroundText.Set("pixel-font", text, { x, y, 0.0f });

    buttons.push_back({ background, foreground, textureDefault, textureSelected });
    return buttons.back();
}

void Menu::Toggle()
{
    auto& reg = Engine::Registry();
    if (isShown)
    {
        isShown = false;
        DestroyMenu();
        for (auto& button : buttons)
        {
            reg.destroy(button.background);
            reg.get<Text>(button.text).Set("pixel-font", "");
            reg.destroy(button.text);
        }
        buttons.clear();
        Engine::ToggleSystemsPause(false);
    }
    else
    {
        isShown = true;
        selected = 0;
        if(shouldPause)
            Engine::ToggleSystemsPause(true);
        CreateMenu();
    }
}
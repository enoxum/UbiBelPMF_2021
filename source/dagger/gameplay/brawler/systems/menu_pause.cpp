#include "menu_pause.h"

#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "core/graphics/text.h"
#include "gameplay/brawler/systems/menu_main.h"
#include "gameplay/brawler/level.h"

using namespace dagger;
using namespace brawler;

MenuPause::MenuPause()
	: Menu(false)
{

}

void MenuPause::CreateMenu()
{
	auto& reg = Engine::Registry();

	/* Background */ {
		background = reg.create();
		auto& backgroundTransform = reg.emplace<Transform>(background);
		backgroundTransform.position = { 0.0f, 0.0f, 0.0f };
		auto& backgroundSprite = reg.emplace<Sprite>(background);
		AssignSprite(backgroundSprite, "EmptyWhitePixel");
		backgroundSprite.UseAsUI();
		backgroundSprite.scale = { 400.0f, 500.0f };
		backgroundSprite.color = { 0.0f, 0.0f, 0.0f, 0.9f };
	}
	Button& bContinue = AddButton(0, 150, "Continue", "ui:button_blue", "ui:button_blue_selected");
	bContinue.onSelected.connect<&MenuPause::OnContinue>(this);
	Button& bRestart = AddButton(0, 0, "Restart", "ui:button_blue", "ui:button_blue_selected");
	bRestart.onSelected.connect<&MenuPause::OnRestart>(this);
	Button& bQuit = AddButton(0, -150, "Quit To Menu", "ui:button_red", "ui:button_red_selected");
	bQuit.onSelected.connect<&MenuPause::OnQuit>(this);
}

void MenuPause::DestroyMenu()
{
	auto& reg = Engine::Registry();

	reg.destroy(background);
}

void MenuPause::OnContinue()
{
	Toggle();
}

void MenuPause::OnRestart()
{
	Toggle();
	Level::Reload();
}

void MenuPause::OnQuit()
{
	Toggle();
	Engine::Registry().clear();
	Engine::GetDefaultResource<MenuMain>()->Toggle();
}

#include "menu_main.h"

#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "core/graphics/text.h"
#include "gameplay/brawler/level.h"

using namespace dagger;
using namespace brawler;

MenuMain::MenuMain()
	: Menu(true)
{

}

void MenuMain::CreateMenu()
{
	auto& reg = Engine::Registry();

	/* Background */ {
		background = reg.create();
		auto& backgroundTransform = reg.emplace<Transform>(background);
		backgroundTransform.position = { 0.0f, 0.0f, 0.0f };
		auto& backgroundSprite = reg.emplace<Sprite>(background);
		AssignSprite(backgroundSprite, "EmptyWhitePixel");
		backgroundSprite.UseAsUI();
		backgroundSprite.scale = { 1280.0f, 720.0f };
		backgroundSprite.color = { 0.0f, 0.0f, 0.0f, 0.9f };
	}
	Button &bPlay = AddButton(0, 100, "Play", "ui:button_blue", "ui:button_blue_selected");
	bPlay.onSelected.connect<&MenuMain::OnPlay>(this);
	Button &bQuit = AddButton(0, -100, "Quit", "ui:button_red", "ui:button_red_selected");
	bQuit.onSelected.connect<&MenuMain::OnQuit>(this);
}

void brawler::MenuMain::DestroyMenu()
{
	auto& reg = Engine::Registry();

	reg.destroy(background);
}

void MenuMain::OnPlay()
{
	Toggle();
	Level::Load("default");
}

void MenuMain::OnQuit()
{
	Engine::Dispatcher().trigger<Exit>();
}

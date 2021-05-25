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
	Button &bPlay1 = AddButton(0, 150, "Play Level 1", "ui:button_blue", "ui:button_blue_selected");
	bPlay1.onSelected.connect<&MenuMain::OnPlayLevel1>(this);
	Button& bPlay2 = AddButton(0, 0, "Play Level 2", "ui:button_blue", "ui:button_blue_selected");
	bPlay2.onSelected.connect<&MenuMain::OnPlayLevel2>(this);
	Button &bQuit = AddButton(0, -150, "Quit", "ui:button_red", "ui:button_red_selected");
	bQuit.onSelected.connect<&MenuMain::OnQuit>(this);
}

void brawler::MenuMain::DestroyMenu()
{
	auto& reg = Engine::Registry();

	reg.destroy(background);
}

void MenuMain::OnPlayLevel1()
{
	Toggle();
	Level::Load("default");
}

void MenuMain::OnPlayLevel2()
{
	Toggle();
	Level::Load("factory");
}

void MenuMain::OnQuit()
{
	Engine::Dispatcher().trigger<Exit>();
}

#include "menu_end.h"

#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "core/graphics/text.h"
#include "gameplay/brawler/systems/menu_main.h"
#include "gameplay/brawler/level.h"

using namespace dagger;
using namespace brawler;

MenuEnd::MenuEnd()
	: Menu(false, false)
{

}

void MenuEnd::CreateMenu()
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
	/* Text */ {
		winner_text = reg.create();
		auto& winnerText = reg.emplace<Text>(winner_text);
		winnerText.letterSize = { 37.0f, 47.0f };
		winnerText.spacing = 0.7;
		winnerText.Set("pixel-font", winner_name + " wins!", { 0.0f, 150.0f, 0.0f });
	}
	Button& bPlayAgain = AddButton(0, 0, "Play Again", "ui:button_blue", "ui:button_blue_selected");
	bPlayAgain.onSelected.connect<&MenuEnd::OnReplay>(this);
	Button& bQuit = AddButton(0, -150, "Quit To Menu", "ui:button_red", "ui:button_red_selected");
	bQuit.onSelected.connect<&MenuEnd::OnQuit>(this);
}

void MenuEnd::DestroyMenu()
{
	auto& reg = Engine::Registry();

	reg.destroy(background);
	auto& text = reg.get<Text>(winner_text);
	text.Set("pixel-font", "");
	reg.destroy(winner_text);

	hasWinner = false;
}

void MenuEnd::OnReplay()
{
	Toggle();
	Level::Reload();
}

void MenuEnd::OnQuit()
{
	Toggle();
	Engine::Registry().clear();
	Engine::GetDefaultResource<MenuMain>()->Toggle();
}

void MenuEnd::Run()
{
	if (hasWinner)
		return;

	auto& player1 = BrawlerCharacter::Get(Level::Player1()).character;
	auto& player2 = BrawlerCharacter::Get(Level::Player2()).character;

	if (player1.health <= 0)
	{
		winner_name = player2.name;
		Toggle();
		hasWinner = true;
	}
	else if (player2.health <= 0)
	{
		winner_name = player1.name;
		Toggle();
		hasWinner = true;
	}
}
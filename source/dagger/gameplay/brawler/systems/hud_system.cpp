#include "hud_system.h"

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include "gameplay/common/simple_collisions.h"

#include "gameplay/brawler/components/weaponpickup.h"
#include "gameplay/brawler/entities/weaponpickup.h"
#include "gameplay/brawler/components/player.h"
#include "gameplay/brawler/components/healthblip.h"
#include "gameplay/brawler/components/weaponblip.h"
#include "gameplay/brawler/systems/bullet_system.h"
#include "gameplay/brawler/brawler_main.h"


using namespace brawler;
using namespace dagger;

float HUDSystem::s_mainWeaponScale		= 1.0f;
float HUDSystem::s_otherWeaponsScale	= 0.5f;
float HUDSystem::s_blipWidth			= 1.0f;
float HUDSystem::s_blipHeight			= 10.0f;
float HUDSystem::s_initialYOffset		= -20.0f;

void brawler::HUDSystem::CreateHealthBar(bool side)
{

	for (int i = 0; i < 100; i++)
	{
		auto entity				= Engine::Registry().create();
		auto& sprite			= Engine::Registry().emplace<Sprite>(entity);
		auto& transform			= Engine::Registry().emplace<Transform>(entity);
		auto& healthBlip		= Engine::Registry().emplace<HealthBlip>(entity);

		healthBlip.setRed();
		healthBlip.idx = i;
		healthBlip.side = side;
		transform.position.z = 0;

		AssignSprite(sprite, "EmptyWhitePixel");

	}

}

void HUDSystem::CreateHealthBarLeft()
{	
	HUDSystem::CreateHealthBar(false);
}

void brawler::HUDSystem::CreateHealthBarRight()
{
	HUDSystem::CreateHealthBar(true);
}

void brawler::HUDSystem::CreateWeaponsLeft()
{
	HUDSystem::CreateWeapons(false);
}

void brawler::HUDSystem::CreateWeaponsRight()
{
	HUDSystem::CreateWeapons(true);
}

void brawler::HUDSystem::CreateWeapons(bool side)
{
	for (int i = 0; i < 5; i++)
	{
		auto entity = Engine::Registry().create();
		auto& sprite = Engine::Registry().emplace<Sprite>(entity);
		auto& transform = Engine::Registry().emplace<Transform>(entity);
		auto& weaponBlip = Engine::Registry().emplace<WeaponBlip>(entity);

		weaponBlip.idx = i;
		weaponBlip.side = side;
		transform.position.z = 0;


		AssignSprite(sprite, "EmptyWhitePixel");
		sprite.color = {1.0f, 1.0f, 1.0f, 0.0f};
	}
}

void brawler::HUDSystem::CreateMainWeaponBlip(bool side)
{
	auto entity = Engine::Registry().create();
	auto& s = Engine::Registry().emplace<Sprite>(entity);
	auto& t = Engine::Registry().emplace<Transform>(entity);
	AssignSprite(s, "EmptyWhitePixel");

	if (side) {
		Brawler::leftMainWeaponBlip = entity;
	}
	else {
		Brawler::rightMainWeaponBlip = entity;
	}
}

void brawler::HUDSystem::CreateLeftMainWeaponBlip()
{
	HUDSystem::CreateMainWeaponBlip(false);
}

void brawler::HUDSystem::CreateRightMainWeaponBlip()
{
	HUDSystem::CreateMainWeaponBlip(true);
}



void HUDSystem::Run()
{

	auto healthBlips = Engine::Registry().view<Sprite, Transform, HealthBlip>();

	auto playerLeft = Engine::Registry().get<Player>(Brawler::leftPlayer);
	auto playerRight = Engine::Registry().get<Player>(Brawler::leftPlayer);

	float offsetY = BulletSystem::s_CameraBoundUp + HUDSystem::s_initialYOffset;

	

	for (auto obj : healthBlips)
	{
		auto& t = healthBlips.get<Transform>(obj);
		auto& s = healthBlips.get<Sprite>(obj);
		auto& h = healthBlips.get<HealthBlip>(obj);
		s.size = { HUDSystem::s_blipWidth, HUDSystem::s_blipHeight };
		s.scale = {1, 1 };

		float offsetX = 0;
		Player player;
		if (!h.side) {
			// 0 - cameraboundleft whenever i find out how to get it
			offsetX = BulletSystem::s_CameraBoundLeft + HUDSystem::s_blipWidth * h.idx;
			player = playerLeft;
		}
		else {
			offsetX = BulletSystem::s_CameraBoundRight - HUDSystem::s_blipWidth * (100 - h.idx);
			player = playerRight;
		}

		if (h.idx <= player.health) {
			h.setRed();
		}
		else {
			h.setWhite();
		}

		s.color = { h.color.x, h.color.y, h.color.z, 1.0f };

		t.position.x = offsetX;
		t.position.y = offsetY;
		s.position = t.position;
	}

	offsetY -= HUDSystem::s_blipHeight;
	float totalHudWidth = 100 * HUDSystem::s_blipWidth;
	float sideWeaponWidth = totalHudWidth / 5;

	auto weaponBlips = Engine::Registry().view<WeaponBlip, Sprite, Transform>();


	for (auto obj : weaponBlips)
	{
		auto& t = weaponBlips.get<Transform>(obj);
		auto& s = weaponBlips.get<Sprite>(obj);
		auto& w = weaponBlips.get<WeaponBlip>(obj);

		
		s.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		s.scale = { 1, 1 };
		s.size = { sideWeaponWidth, HUDSystem::s_otherWeaponsScale };
		
		float offsetX = 0;
		Player player;
		if (!w.side)
		{
			player = playerLeft;
			offsetX = BulletSystem::s_CameraBoundLeft + sideWeaponWidth * w.idx;
		}
		else {
			player = playerRight;
			offsetX = BulletSystem::s_CameraBoundRight - sideWeaponWidth * (5 - w.idx);
		}

		if (w.idx >= player.weapons.size()) {
			s.color = { 1.0f, 1.0f, 1.0f, 0.0f };
			continue;
		}

		t.position.x = offsetX;
		t.position.y = offsetY;

		AssignSprite(s, "brawler:" + player.weapons[w.idx].sprite());

		


	}



		
}
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
#include "gameplay/brawler/components/weaponmarker.h"
#include "gameplay/brawler/systems/bullet_system.h"
#include "gameplay/brawler/brawler_main.h"


using namespace brawler;
using namespace dagger;

float HUDSystem::s_mainWeaponScale				= 1.0f;
float HUDSystem::s_weaponDim					= 20.0f;
float HUDSystem::s_blipWidth					= 1.0f;
float HUDSystem::s_blipHeight					= 10.0f;
float HUDSystem::s_activeWeaponIndicatorSize	= 10.0f;
float HUDSystem::s_miscPadding					= 10.0f;

float HUDSystem::s_paddingUp	 = 20;
float HUDSystem::s_paddingSide	 = 20;

int HUDSystem::s_playerHealth = 100;


void HUDSystem::CreateHealthBar(bool side)
{

	for (int i = 0; i < 100; i++)
	{
		auto entity				= Engine::Registry().create();
		auto& sprite			= Engine::Registry().emplace<Sprite>(entity);
		auto& transform			= Engine::Registry().emplace<Transform>(entity);
		auto& healthBlip		= Engine::Registry().emplace<HealthBlip>(entity);

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

void HUDSystem::CreateWeapons(bool side)
{
	auto leftMarker		= Engine::Registry().create();
	auto rightMarker	= Engine::Registry().create();
	auto& wml = Engine::Registry().emplace<WeaponMarker>(leftMarker);
	auto& wmr = Engine::Registry().emplace<WeaponMarker>(rightMarker);
	wml.side = false;
	wmr.side = true;
	Engine::Registry().emplace<Transform>(leftMarker);
	auto& ls = Engine::Registry().emplace<Sprite>(leftMarker);
	AssignSprite(ls, "brawler:marker");
	Engine::Registry().emplace<Transform>(rightMarker);
	auto& rs = Engine::Registry().emplace<Sprite>(rightMarker);
	AssignSprite(rs, "brawler:marker");
	
	
	ls.color = rs.color = { 1.0f, 1.0f, 1.0f, 0.0f };

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

void HUDSystem::CreateMainWeaponBlip(bool side)
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

void HUDSystem::CreateLeftMainWeaponBlip()
{
	HUDSystem::CreateMainWeaponBlip(false);
}

void HUDSystem::CreateRightMainWeaponBlip()
{
	HUDSystem::CreateMainWeaponBlip(true);
}



void HUDSystem::Run()
{

	auto healthBlips = Engine::Registry().view<Sprite, Transform, HealthBlip>();

	auto playerLeft = Engine::Registry().get<Player>(Brawler::leftPlayer);
	auto playerRight = Engine::Registry().get<Player>(Brawler::leftPlayer);

	playerLeft.health = HUDSystem::s_playerHealth;

	if (playerLeft.active_weapon_idx == -1) {
		Engine::Registry().view<WeaponMarker, Sprite, Transform>().each([&](WeaponMarker& wpMarker, Sprite& wpMarkerSprite, Transform& wpMarkerTransform) {
			if (wpMarker.side)
				return;
			wpMarkerSprite.color = { 1.0f, 1.0f, 1.0f, 0.0f };
		});
	}

	if (playerRight.active_weapon_idx == -1) {
		Engine::Registry().view<WeaponMarker, Sprite, Transform>().each([&](WeaponMarker& wpMarker, Sprite& wpMarkerSprite, Transform& wpMarkerTransform) {
			if (!wpMarker.side)
				return;
			wpMarkerSprite.color = { 1.0f, 1.0f, 1.0f, 0.0f };
		});
	}


	float offsetY = BulletSystem::s_CameraBoundUp - HUDSystem::s_paddingUp;
	
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
			offsetX = BulletSystem::s_CameraBoundLeft +  HUDSystem::s_blipWidth * h.idx;
			if (h.idx == 0) {
				offsetX += HUDSystem::s_paddingSide;
			}
			player = playerLeft;
		}
		else {
			offsetX = BulletSystem::s_CameraBoundRight - HUDSystem::s_blipWidth * (99 - h.idx);
			player = playerRight;
			if (h.idx == 99) {
				offsetX -= HUDSystem::s_paddingSide;
			}
		}

		if ((!h.side && h.idx <= player.health)
			|| (h.side && (99 - h.idx <= player.health))) {
			s.color = { 1.0f, 0.0f, 0.0f, 1.0f };
		}
		else {
			s.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		}



		t.position.x = offsetX;
		t.position.y = offsetY;
		s.position = t.position;
	}

	offsetY -= HUDSystem::s_blipHeight + HUDSystem::s_paddingUp;
	float totalHudWidth = 100 * HUDSystem::s_blipWidth;
	float sideWeaponWidth = totalHudWidth / 5;

	auto weaponBlips = Engine::Registry().view<WeaponBlip, Sprite, Transform>();


	for (auto obj : weaponBlips)
	{
		auto& t = weaponBlips.get<Transform>(obj);
		auto& s = weaponBlips.get<Sprite>(obj);
		auto& w = weaponBlips.get<WeaponBlip>(obj);

		Player player;
		float y = 0;
		if (!w.side)
		{
			player = playerLeft;
			y = offsetY - HUDSystem::s_weaponDim * w.idx;
		}
		else {
			player = playerRight;
			//todo: y

		}



		if (w.idx >= player.weapons.size()) {
			AssignSprite(s, "EmptyWhitePixel");
			s.color = { 1.0f, 1.0f, 1.0f, 0.0f };
			continue;
		}
		if (!w.side) {
			t.position.x = BulletSystem::s_CameraBoundLeft + HUDSystem::s_paddingSide;
		}
		else {
			t.position.x = BulletSystem::s_CameraBoundRight - HUDSystem::s_paddingSide;
		}
		

		if (player.active_weapon_idx == w.idx)
		{
			
			Engine::Registry().view<WeaponMarker, Sprite, Transform>().each([&](WeaponMarker& wpMarker, Sprite& wpMarkerSprite, Transform& wpMarkerTransform) {
				if (wpMarker.side != w.side)
					return;
				wpMarkerSprite.color = { 1.0f, 1.0f, 1.0f, 1.0f };
				wpMarkerSprite.position = wpMarkerTransform.position = t.position;
				wpMarkerSprite.scale = { (HUDSystem::s_activeWeaponIndicatorSize) / wpMarkerSprite.size.x , (HUDSystem::s_activeWeaponIndicatorSize) / wpMarkerSprite.size.y };
			});
			if (!w.side) {
				t.position.x += HUDSystem::s_activeWeaponIndicatorSize + HUDSystem::s_paddingSide;
			}
			else {
				t.position.x -= HUDSystem::s_activeWeaponIndicatorSize + HUDSystem::s_paddingSide;
			}
			
		}

		t.position.y = y;
		AssignSprite(s, "brawler:" + player.weapons[w.idx].sprite());
		s.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		s.position = t.position;
		s.scale = { HUDSystem::s_weaponDim/s.size.x, HUDSystem::s_weaponDim/s.size.y };

		

	
		
		



	}


	



		
}
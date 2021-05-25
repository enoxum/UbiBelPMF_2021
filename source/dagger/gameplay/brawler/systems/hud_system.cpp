#include "hud_system.h"

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/text.h"

#include "gameplay/common/simple_collisions.h"

#include "gameplay/brawler/components/weaponpickup.h"
#include "gameplay/brawler/components/player.h"
#include "gameplay/brawler/components/healthbar.h"
#include "gameplay/brawler/components/weaponblip.h"
#include "gameplay/brawler/entities/weaponpickup.h"
#include "gameplay/brawler/systems/bullet_system.h"
#include "gameplay/brawler/level.h"

using namespace brawler;
using namespace dagger;

float HUDSystem::s_weaponDim					= 20.0f;
float HUDSystem::s_blipWidth					= 2.2f;
float HUDSystem::s_blipHeight					= 15.0f;
float HUDSystem::s_activeWeaponIndicatorSize	= 10.0f;

float HUDSystem::s_paddingUp		= 20;
float HUDSystem::s_paddingSide		= 12;
float HUDSystem::s_midPaddingUp		= 7.5f;
float HUDSystem::s_midPaddingSide	= 7.5f;

int HUDSystem::s_leftPlayerHealth = 100;
int HUDSystem::s_rightPlayerHealth = 100;

Entity HUDSystem::leftMarker{ entt::null };
Entity HUDSystem::rightMarker{ entt::null };
Entity HUDSystem::leftText{ entt::null };
Entity HUDSystem::rightText{ entt::null };

void HUDSystem::Init()
{
	CreateHealthBarLeft();
	CreateWeaponsLeft();

	CreateHealthBarRight();
	CreateWeaponsRight();

	CreateMarkersAndTexts();
}

void HUDSystem::CreateHealthBar(bool side)
{
	auto lr = Engine::Registry().create();
	auto lw = Engine::Registry().create();
	auto rr = Engine::Registry().create();
	auto rw = Engine::Registry().create();

	//Engine::Registry().emplace<Transform>(lr);
	auto& lrh = Engine::Registry().emplace<HealthBar>(lr);
	lrh.side = false;
	lrh.color = true;

	//Engine::Registry().emplace<Transform>(lw);
	auto& lwh = Engine::Registry().emplace<HealthBar>(lw);
	lwh.side = false;
	lwh.color = false;

	//Engine::Registry().emplace<Transform>(rr);
	auto& rrh = Engine::Registry().emplace<HealthBar>(rr);
	rrh.side = true;
	rrh.color = true;

	//Engine::Registry().emplace<Transform>(rw);
	auto& rwh = Engine::Registry().emplace<HealthBar>(rw);
	rwh.side = true;
	rwh.color = false;

	auto& lrt = Engine::Registry().emplace<Transform>(lr);
	auto& lrs = Engine::Registry().emplace<Sprite>(lr);
	AssignSprite(lrs, "EmptyWhitePixel");
	auto& lwt = Engine::Registry().emplace<Transform>(lw);
	auto& lws = Engine::Registry().emplace<Sprite>(lw);
	AssignSprite(lws, "EmptyWhitePixel");
	auto& rrt = Engine::Registry().emplace<Transform>(rr);
	auto& rrs = Engine::Registry().emplace<Sprite>(rr);
	AssignSprite(rrs, "EmptyWhitePixel");
	auto& rwt = Engine::Registry().emplace<Transform>(rw);
	auto& rws = Engine::Registry().emplace<Sprite>(rw);
	AssignSprite(rws, "EmptyWhitePixel");
}

void HUDSystem::CreateHealthBarLeft()
{	
	HUDSystem::CreateHealthBar(false);
}

void HUDSystem::CreateHealthBarRight()
{
	HUDSystem::CreateHealthBar(true);
}

void HUDSystem::CreateWeaponsLeft()
{
	HUDSystem::CreateWeapons(false);
}

void HUDSystem::CreateWeaponsRight()
{
	HUDSystem::CreateWeapons(true);
}

void HUDSystem::CreateWeapons(bool side)
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

void HUDSystem::CreateMarkersAndTexts()
{
	HUDSystem::leftMarker = Engine::Registry().create();
	HUDSystem::rightMarker = Engine::Registry().create();
	Engine::Registry().emplace<Transform>(HUDSystem::leftMarker);
	auto& ls = Engine::Registry().emplace<Sprite>(HUDSystem::leftMarker);
	AssignSprite(ls, "brawler:arrow_right");

	Engine::Registry().emplace<Transform>(HUDSystem::rightMarker);
	auto& rs = Engine::Registry().emplace<Sprite>(HUDSystem::rightMarker);
	AssignSprite(rs, "brawler:arrow_left");
	ls.color = rs.color = { 1.0f, 1.0f, 1.0f, 0.0f };
	
	HUDSystem::leftText = Engine::Registry().create();
	auto& lt = Engine::Registry().emplace<Text>(HUDSystem::leftText);
	lt.spacing = 0.6f;
	lt.Set("pixel-font", "", { 0, 0, 0 }, false);

	HUDSystem::rightText = Engine::Registry().create();
	auto& rt = Engine::Registry().emplace<Text>(HUDSystem::rightText);
	rt.spacing = 0.6f;
	rt.Set("pixel-font", "", { 0, 0, 0 }, false);
}

void HUDSystem::Run()
{
	auto playerLeft = Engine::Registry().get<Player>(Level::Player1());
	auto playerRight = Engine::Registry().get<Player>(Level::Player2());

//	playerLeft.health = HUDSystem::s_leftPlayerHealth;
//	playerRight.health = HUDSystem::s_rightPlayerHealth;

	auto& leftText	= Engine::Registry().get<Text>(HUDSystem::leftText);
	auto& rightText = Engine::Registry().get<Text>(HUDSystem::rightText);

	auto& leftMarkerSprite = Engine::Registry().get<Sprite>(HUDSystem::leftMarker);
	auto& rightMarkerSprite = Engine::Registry().get<Sprite>(HUDSystem::rightMarker);
	auto& leftMarkerTransform = Engine::Registry().get<Transform>(HUDSystem::leftMarker);
	auto& rightMarkerTransform = Engine::Registry().get<Transform>(HUDSystem::rightMarker);

	if (playerLeft.active_weapon_idx == -1) {
		leftMarkerSprite.color = { 1.0f, 1.0f, 1.0f, 0.0f };
		leftText.Set("pixel-font", "", { 0, 0, 0 }, false);
	}

	if (playerRight.active_weapon_idx == -1) {
		rightMarkerSprite.color = { 1.0f, 1.0f, 1.0f, 0.0f };
		rightText.Set("pixel-font", "", { 0, 0, 0 }, false);
	}

	float offsetY = BulletSystem::s_CameraBoundUp - HUDSystem::s_paddingUp - HUDSystem::s_blipHeight / 2;
	Engine::Registry().view<HealthBar, Transform, Sprite>().each([&](HealthBar& hb, Transform& hbTransform, Sprite& hbSprite) {
		
		float whitePartWidth = 100 * HUDSystem::s_blipWidth;;
		float whiteX = 0;

		float redPartWidth = 0;
		float redX   = 0;	
		
		if (!hb.side)
		{
			redPartWidth = playerLeft.health * HUDSystem::s_blipWidth;
			redX = BulletSystem::s_CameraBoundLeft + HUDSystem::s_paddingSide + redPartWidth / 2;
			whiteX = BulletSystem::s_CameraBoundLeft + HUDSystem::s_paddingSide + whitePartWidth / 2;
		}
		else {
			redPartWidth = playerRight.health * HUDSystem::s_blipWidth;
			redX = BulletSystem::s_CameraBoundRight - HUDSystem::s_paddingSide - redPartWidth / 2;
			whiteX = BulletSystem::s_CameraBoundRight - HUDSystem::s_paddingSide - whitePartWidth / 2;
		}

		if (hb.color)
		{
			// Red
			hbTransform.position = { redX, offsetY, 0.0f };
			hbSprite.scale = { redPartWidth / hbSprite.size.x, HUDSystem::s_blipHeight / hbSprite.size.y};
			hbSprite.color = { 1.0f, 0.0f, 0.0f, 1.0f };
			hbSprite.position = { redX, offsetY, 0.0f };
		}
		else
		{
			// White
			hbTransform.position = { whiteX, offsetY, 1.0f };
			hbSprite.scale = { whitePartWidth / hbSprite.size.x , HUDSystem::s_blipHeight  / hbSprite.size.y};
			hbSprite.color = { 1.0f, 1.0f, 1.0f, 1.0f };
			hbSprite.position = { whiteX, offsetY, 1.0f };
		}

	});

	offsetY -= HUDSystem::s_blipHeight + HUDSystem::s_midPaddingUp;
	float totalHudWidth = 100 * HUDSystem::s_blipWidth;
	float sideWeaponWidth = totalHudWidth / 5;

	auto weaponBlips = Engine::Registry().view<WeaponBlip, Sprite, Transform>();

	for (auto obj : weaponBlips)
	{
		auto& t = weaponBlips.get<Transform>(obj);
		auto& s = weaponBlips.get<Sprite>(obj);
		auto& w = weaponBlips.get<WeaponBlip>(obj);

		Player player;
		float y = offsetY - HUDSystem::s_weaponDim * w.idx;
		if (!w.side)
		{
			player = playerLeft;
			t.position.x = BulletSystem::s_CameraBoundLeft + HUDSystem::s_paddingSide;
		}
		else
		{
			player = playerRight;
			t.position.x = BulletSystem::s_CameraBoundRight - HUDSystem::s_paddingSide;
		}

		if (w.idx >= player.weapons.size()) {
			AssignSprite(s, "EmptyWhitePixel");
			s.color = { 1.0f, 1.0f, 1.0f, 0.0f };
			continue;
		}

		if (player.active_weapon_idx == w.idx)
		{
			
			if (!w.side) {
				leftMarkerSprite.color = { 1.0f, 1.0f, 1.0f, 1.0f };
				leftMarkerSprite.position = leftMarkerTransform.position = t.position;
				leftMarkerSprite.scale = { (HUDSystem::s_activeWeaponIndicatorSize) / leftMarkerSprite.size.x , (HUDSystem::s_activeWeaponIndicatorSize) / leftMarkerSprite.size.y };
				t.position.x += HUDSystem::s_activeWeaponIndicatorSize + HUDSystem::s_midPaddingSide;
				leftText.spacing = 0.25f;
				leftText.alignment = TextAlignment::LEFT;
				leftText.letterSize = { static_cast<float>(HUDSystem::s_weaponDim) / 2, static_cast<float>(HUDSystem::s_weaponDim) / 2 };
				leftText.Set("pixel-font", playerLeft.weapons[playerLeft.active_weapon_idx].ammoRepr(), { t.position.x + HUDSystem::s_weaponDim + HUDSystem::s_midPaddingSide, y, 0 }, false);

			}
			else {
				rightMarkerSprite.color = { 1.0f, 1.0f, 1.0f, 1.0f };
				rightMarkerSprite.position = rightMarkerTransform.position = t.position;
				rightMarkerSprite.scale = { (HUDSystem::s_activeWeaponIndicatorSize) / rightMarkerSprite.size.x , (HUDSystem::s_activeWeaponIndicatorSize) / rightMarkerSprite.size.y };
				t.position.x -= HUDSystem::s_activeWeaponIndicatorSize + HUDSystem::s_midPaddingSide;
				rightText.spacing = 0.25f;
				rightText.alignment = TextAlignment::RIGHT;
				rightText.letterSize = { static_cast<float>(HUDSystem::s_weaponDim) / 2, static_cast<float>(HUDSystem::s_weaponDim) / 2 };
				rightText.Set("pixel-font", playerRight.weapons[playerRight.active_weapon_idx].ammoRepr(), { t.position.x - HUDSystem::s_weaponDim - HUDSystem::s_midPaddingSide, y, 0 }, false);
			}
			
		}

		t.position.y = y;
		AssignSprite(s, "brawler:" + player.weapons[w.idx].sprite());
		s.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		s.scale = { HUDSystem::s_weaponDim/s.size.x, HUDSystem::s_weaponDim/s.size.y };
	}
		
}
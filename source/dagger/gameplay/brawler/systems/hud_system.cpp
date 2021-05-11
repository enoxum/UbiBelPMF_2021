#include "hud_system.h"

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include "gameplay/common/simple_collisions.h"

#include "gameplay/brawler/components/weaponpickup.h"
#include "gameplay/brawler/entities/weaponpickup.h"
#include "gameplay/brawler/components/player.h"


using namespace brawler;
using namespace dagger;

float HUDSystem::s_mainWeaponScale		= 1.0f;
float HUDSystem::s_otherWeaponsScale	= 0.5f;

void HUDSystem::Run()
{

}
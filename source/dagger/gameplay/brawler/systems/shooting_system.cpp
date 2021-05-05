#include "shooting_system.h"

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include "gameplay/brawler/components/bullet.h"
#include "gameplay/brawler/components/movable.h"
#include "gameplay/brawler/components/player.h"
#include "gameplay/brawler/entities/bullet.h"

using namespace brawler;
using namespace dagger;

void ShootingSystem::Run()
{

	auto view = Engine::Registry().view<Sprite, InputReceiver, Player, Transform, Movable>();

    view.each([](auto &sprite, auto &input, auto& player, auto& transform, auto& movable) {
        // TODO cooldown
        if (EPSILON_NOT_ZERO(input.Get("attack")))
        {
            // recoil
            // TODO set based on weapon
            movable.speed.x -= sprite.scale.x * BulletSystem::s_PlayerRecoil;
	        BulletEntity::Create(transform.position, sprite.scale.x>=0.0f? 1 : -1);
        }
    });
}

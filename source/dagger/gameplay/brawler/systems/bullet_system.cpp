#include "bullet_system.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "core/graphics/window.h"
#include "core/graphics/animation.h"

#include "gameplay/brawler/components/movable.h"
#include "gameplay/brawler/components/bullet.h"

using namespace brawler;
using namespace dagger;

int BulletSystem::s_ActiveBullets = 0;
float BulletSystem::s_BulletSpeed = 500.0f;
float BulletSystem::s_PlayerRecoil = 20.0f;

float BulletSystem::s_CameraBoundLeft = 0;
float BulletSystem::s_CameraBoundRight = 0;
float BulletSystem::s_CameraBoundUp = 0;
float BulletSystem::s_CameraBoundDown = 0;

void BulletSystem::Run()
{

	auto  bullets = Engine::Registry().view<Sprite, Transform, Bullet>();
	auto  projectiles = Engine::Registry().view<Sprite, Transform, Bullet, Movable, Animator>();
	auto* camera = Engine::GetDefaultResource<Camera>();
	
	BulletSystem::s_CameraBoundLeft = camera->position.x - camera->size.x / 4;
	BulletSystem::s_CameraBoundRight = camera->position.x + camera->size.x / 4;
	BulletSystem::s_CameraBoundUp = camera->position.y + camera->size.y / 4;
	BulletSystem::s_CameraBoundDown = camera->position.y - camera->size.y / 4;

	auto& reg = Engine::Registry();
	for (auto obj : bullets) {
		
		auto& t = bullets.get<Transform>(obj);
		auto& b = bullets.get<Bullet>(obj);

		if(b.type == 0){
			Vector3 dp = { b.direction*BulletSystem::s_BulletSpeed, 0, 0 };
			t.position += dp * Engine::DeltaTime();
			t.position.z = 1.0f;
		
			if (t.position.x < BulletSystem::s_CameraBoundLeft || t.position.x > BulletSystem::s_CameraBoundRight) {
				reg.destroy(obj);
				BulletSystem::s_ActiveBullets--;
			}
		}
	}

	for (auto obj : projectiles) {

		auto& t = projectiles.get<Transform>(obj);
		auto& b = projectiles.get<Bullet>(obj);
		auto& a = projectiles.get<Animator>(obj);
		
		if (t.position.x < BulletSystem::s_CameraBoundLeft || t.position.x > BulletSystem::s_CameraBoundRight || t.position.y == 0) {
			AnimatorPlay(a, "EXPLOSION");
		}

		// Waiting for merge
		if(a.currentFrame == 24){
			reg.destroy(obj);
			BulletSystem::s_ActiveBullets--;
		}
	}
}

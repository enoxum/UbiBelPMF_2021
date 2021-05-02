#include "bullet_system.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "core/graphics/window.h"

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

	auto  objects = Engine::Registry().view<Sprite, Transform, Bullet>();
	auto* camera = Engine::GetDefaultResource<Camera>();
	
	BulletSystem::s_CameraBoundLeft = camera->position.x - camera->size.x / 2;
	BulletSystem::s_CameraBoundRight = camera->position.x + camera->size.x / 2;
	BulletSystem::s_CameraBoundUp = camera->position.y - camera->size.y / 2;
	BulletSystem::s_CameraBoundDown = camera->position.y + camera->size.y / 2;

	auto& reg = Engine::Registry();
	for (auto obj : objects) {

		auto& t = objects.get<Transform>(obj);
		auto& b = objects.get<Bullet>(obj);

		Vector3 dp = { b.direction*BulletSystem::s_BulletSpeed, 0, 0 };
		t.position += dp * Engine::DeltaTime();

		if (t.position.x < BulletSystem::s_CameraBoundLeft || t.position.x > BulletSystem::s_CameraBoundRight) {
			reg.destroy(obj);
			
			BulletSystem::s_ActiveBullets--;
		}

	}

}

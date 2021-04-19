#include "bullet_system.h"


#include "core/engine.h"
#include "core/game/transforms.h"
#include "../components/bullet.h"
#include "core/graphics/sprite.h"
#include <core/graphics/window.h>

using namespace brawler;
using namespace dagger;

float BulletSystem::bulletSpeed = 50.0f;
int BulletSystem::activeBullets = 0;

float BulletSystem::cameraBoundLeft = 0;
float BulletSystem::cameraBoundRight = 0;
float BulletSystem::cameraBoundUp = 0;
float BulletSystem::cameraBoundDown = 0;

void BulletSystem::Run()
{

	auto  objects = Engine::Registry().view<Sprite, Transform, Bullet>();
	auto* camera = Engine::GetDefaultResource<Camera>();
	
	BulletSystem::cameraBoundLeft = camera->position.x - camera->size.x / 2;
	BulletSystem::cameraBoundRight = camera->position.x + camera->size.x / 2;
	BulletSystem::cameraBoundUp = camera->position.y - camera->size.y / 2;
	BulletSystem::cameraBoundDown = camera->position.y + camera->size.y / 2;
	auto& reg = Engine::Registry();
	for (auto obj : objects) {
		auto& t = objects.get<Transform>(obj);
		auto& b = objects.get<Bullet>(obj);

		Vector3 dp = { b.direction*BulletSystem::bulletSpeed, 0, 0 };
		t.position += dp * Engine::DeltaTime();

		if (t.position.x < BulletSystem::cameraBoundLeft || t.position.x > BulletSystem::cameraBoundRight) {
			reg.destroy(obj);
			
			BulletSystem::activeBullets--;
		}

		
	}

}

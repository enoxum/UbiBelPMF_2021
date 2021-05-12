#pragma once

#include "core/core.h"
#include "core/engine.h"
#include "core/system.h"

using namespace dagger;

namespace brawler
{

class MapCollisionSystem : public System
{
	String SystemName() override {
		return "Map Collision System";
	}

	void Run() override;
};

}
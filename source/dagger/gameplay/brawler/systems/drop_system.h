#pragma once

#include "core/core.h"
#include "core/engine.h"
#include "core/system.h"

using namespace dagger;

namespace brawler
{

class DropSystem : public System
{
	String SystemName() override {
		return "Drop System";
	}

	void Run() override;
};

}
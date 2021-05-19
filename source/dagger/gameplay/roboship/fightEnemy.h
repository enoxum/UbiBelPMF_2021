#pragma once

#include "core/core.h"
#include "core/game.h"
#include "gameplay/roboship/selectTileController.h"
#include "gameplay/roboship/inventorySetup.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/audio.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite_render.h"
#include "core/graphics/textures.h"
#include "core/graphics/animations.h"
#include "core/graphics/gui.h"
#include "core/graphics/text.h"
#include <vector>


#include "tools/diagnostics.h"

using namespace dagger;
using namespace inventory;

namespace fightEnemy
{

	bool findCombination(std::vector<int> comb);
	std::pair<int, int> checkNeighbor(int i, int j, int k, int len, std::vector<int> comb);
	void destroyTile();
	void changeTiles(int a, int b, int c, int d);
	void addShipPart();
}



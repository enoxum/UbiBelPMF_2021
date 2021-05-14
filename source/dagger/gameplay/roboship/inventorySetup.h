#pragma once

#include "core/core.h"
#include "core/game.h"
#include "gameplay/roboship/selectTileController.h"

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

using MatrixInv = std::vector<std::vector<int>>;

namespace inventory
{
    struct InventoryMatrix
    {
        MatrixInv matrix;
    };

    class Inventory
    {
        
        int height = 4;
        int width = 4;
        float tileSize = 30.f;//  static_cast<float>(Width);

        float zPos = 1.f;

        float Space = 0.3f;

    public:
        void InventoryPositionsSetup();
        void SelectedTileSetup();
        void FillInventory();
        void SwapMatrix(int x, int y, int a, int b);
        void makeMatrix();
        void SwapSprites(int x, int y, int a, int b);
    };
}
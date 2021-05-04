#pragma once

#include "core/core.h"
#include "core/engine.h"

using namespace dagger;

namespace brawler {

    enum PlatformType 
    {
        EMPTY,
        BLOCK
    };

    using Tilemap = std::vector<std::vector<PlatformType>>;
    using TileCoords = std::pair<int, int>;

    class Level
    {
    public:
        static void Create();
        static TileCoords WorldToTile(Vector2 worldPos);
        static PlatformType getTile(int x, int y);
    private:
        static Tilemap tiles;
    };

}
#include "level.h"

#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"

using namespace dagger;
using namespace brawler;

Tilemap Level::tiles{};

void Level::Create()
{
    auto& reg = Engine::Registry();

    unsigned mapWidth = 20;
    unsigned mapHeight = 15;
    float tileWidth = 20.0f;
    float tileHeight = 20.0f;

    // Fill the tilemap
    // TODO Load from json asset
    for (unsigned y = 0; y < mapHeight; y++) {
        tiles.emplace_back();
        for(unsigned x = 0; x < mapWidth; x++) {
            tiles[y].push_back(((x+y)%2==1) && y<4 ? PlatformType::BLOCK : PlatformType::EMPTY);
        }
    }

    // Create tilemap entites
    for (unsigned x = 0; x < mapWidth; x++) {
        for(unsigned y = 0; y < mapHeight; y++) {
            if(tiles[y][x]==PlatformType::BLOCK) {
                auto tile = reg.create();
                auto& sprite = reg.get_or_emplace<Sprite>(tile);
                AssignSprite(sprite, "EmptyWhitePixel");
                sprite.color = { 1, 1, 1, 1 };
                sprite.size = { tileWidth, tileHeight };
                sprite.scale = { 1, 1 };
                sprite.position = { x*tileWidth + tileWidth/2, y*tileHeight + tileHeight/2, 1 };
            }
        }
    }
}

TileCoords Level::WorldToTile(Vector2 worldPos)
{
    // TODO
    return std::make_pair(0,0);
}

PlatformType Level::getTile(int x, int y)
{
    return tiles[y][x];
}
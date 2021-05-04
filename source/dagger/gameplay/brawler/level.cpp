#include "level.h"

#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"

using namespace dagger;
using namespace brawler;

Tilemap Level::tiles{};

void Level::Create()
{
    auto& reg = Engine::Registry();

    // Fill the tilemap
    // TODO Load from json asset
    for (unsigned y = 0; y < LEVEL_HEIGHT; y++) {
        tiles.emplace_back();
        for(unsigned x = 0; x < LEVEL_WIDTH; x++) {
            tiles[y].push_back(((x+y)%2==1) && y<4 ? PlatformType::BLOCK : PlatformType::EMPTY);
        }
    }

    // Create tilemap entites
    for (unsigned x = 0; x < LEVEL_WIDTH; x++) {
        for(unsigned y = 0; y < LEVEL_HEIGHT; y++) {
            if(getTile(x, y) == PlatformType::BLOCK) {
                auto tile = reg.create();
                auto& sprite = reg.get_or_emplace<Sprite>(tile);
                AssignSprite(sprite, "EmptyWhitePixel");
                sprite.color = { 1, 1, 1, 1 };
                sprite.size = { TILE_WIDTH, TILE_HEIGHT };
                sprite.scale = { 1, 1 };
                sprite.position = { TileToWorld(x, y), 1 };
            }
        }
    }
}

std::optional<float> Level::getGround(BrawlerCharacter c) {
    // TODO use a bounding box instead of sprite
    // zoom is set to 2 so sprite size is double the world size
    Vector2 bottomLeft = {c.transform.position.x - c.sprite.size.x/4.0f, c.transform.position.y - c.sprite.size.y/2.0f};
    Vector2 bottomRight = {c.transform.position.x + c.sprite.size.x/4.0f, c.transform.position.y - c.sprite.size.y/2.0f};
    
    auto newY = c.transform.position.y;
    bool changed = false;
    for (Vector2 checkedTile = bottomLeft; checkedTile.x <= bottomRight.x; checkedTile.x += TILE_WIDTH)
    {
        checkedTile.x = checkedTile.x < bottomRight.x ? checkedTile.x : bottomRight.x;

        auto [x, y] = WorldToTile(checkedTile);
        // Logger::info("{0} {1}", x, y);
        
        if(x<0 || x>=LEVEL_WIDTH || y<0 || y>=LEVEL_HEIGHT)
            continue;
        if(Level::getTile(x, y)==PlatformType::BLOCK) {
            float tileY = TileToWorldY(y) + c.sprite.size.y/2;
            if(newY < tileY ) {
                newY = tileY;
                changed = true;
            }
        }
    }
    if (changed)
        return newY;
    return {};
}

TileCoords Level::WorldToTile(Vector2 worldPos)
{
    return { worldPos.x / TILE_WIDTH, worldPos.y / TILE_HEIGHT };
}

Vector2 Level::TileToWorld(int x, int y)
{
    return {
        TileToWorldX(x),
        TileToWorldY(y)
    };
}

float Level::TileToWorldX(int x)
{
    return x * TILE_HEIGHT + TILE_HEIGHT/2;
}

float Level::TileToWorldY(int y)
{
    return y * TILE_HEIGHT + TILE_HEIGHT/2;
}

PlatformType Level::getTile(int x, int y)
{
    return tiles[y][x];
}
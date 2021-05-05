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
    Vector2 oldBottomLeft = {c.movable.prevPosition.x - c.sprite.size.x/4.0f, c.movable.prevPosition.y - c.sprite.size.y/2.0f - 1};
    Vector2 newBottomLeft = {c.transform.position.x - c.sprite.size.x/4.0f, c.transform.position.y - c.sprite.size.y/2.0f - 1};

    int endY = WorldToTileY(newBottomLeft.y);
    int startY = std::max<int>(WorldToTileY(oldBottomLeft.y), endY);
    int distance = std::max<int>(std::abs(endY - startY), 1);

    for (int tileY = startY; tileY >= endY; tileY--)
    {
        Vector2 bottomLeft = newBottomLeft + (std::abs(static_cast<float>(endY - tileY)) / distance) * (oldBottomLeft - newBottomLeft);
        Vector2 bottomRight = bottomLeft + Vector2(c.sprite.size.x/2.0f, 0);

        for (Vector2 checkedTile = bottomLeft; checkedTile.x < bottomRight.x; checkedTile.x += TILE_WIDTH)
        {
            checkedTile.x = std::min<float>(checkedTile.x, bottomRight.x);

            auto [x, y] = WorldToTile(checkedTile);
            // Logger::info("{0} {1}", x, y);
            
            // TODO Move to getTile
            if(x<0 || x>=LEVEL_WIDTH || y<0 || y>=LEVEL_HEIGHT)
                continue;
            if(Level::getTile(x, y)==PlatformType::BLOCK) {
                float ground = TileToWorldY(y) + TILE_HEIGHT/2 + c.sprite.size.y/2;
                return {ground};
            }

            if(checkedTile.x >= bottomRight.x)
                break;
        }
    }

    return {};
}

TileCoords Level::WorldToTile(Vector2 worldPos)
{
    return { WorldToTileX(worldPos.x), WorldToTileY(worldPos.y) };
}

int Level::WorldToTileX(float x)
{
    return static_cast<int>(x) / TILE_WIDTH;
}

int Level::WorldToTileY(float y)
{
    return static_cast<int>(y) / TILE_HEIGHT;
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
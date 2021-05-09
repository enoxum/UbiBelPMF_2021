#pragma once

#include "core/core.h"
#include "core/engine.h"
#include "core/system.h"
#include "gameplay/brawler/level.h"

using namespace dagger;

namespace brawler {

struct TextureData
{
    String name;
    float scale;
};

struct TileData
{
    PlatformType type;
    TextureData texture;
};

struct LevelData
{
    String name;
    unsigned mapWidth;
    unsigned mapHeight;
    Sequence<TileData> tileset;
    Sequence<Sequence<int>> tilemap;
    Sequence<TextureData> backgrounds;
};

class LevelSystem : public System
{
public:
    String SystemName() override {
        return "Level System";
    }

    void LoadDefaultAssets();
    void OnAssetLoadRequest(AssetLoadRequest<LevelData> request_);

    void SpinUp() override;
    void Run() override;
    void WindDown() override;
private:
    TextureData LoadTexture(JSON::json& input_);
};

}
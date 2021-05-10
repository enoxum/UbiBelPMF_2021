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

struct PlayerData
{
	int x;
	int y;
	bool isLeft;
};

struct LevelData
{
	String name;
	unsigned mapWidth;
	unsigned mapHeight;
	Sequence<TileData> tileset;
	Sequence<Sequence<int>> tilemap;
	Sequence<TextureData> backgrounds;
	PlayerData player1;
	PlayerData player2;
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
	PlayerData LoadPlayer(JSON::json& player_);
};

}
#pragma once

#include "core/core.h"
#include "core/engine.h"
#include "gameplay/brawler/entities/character.h"
#include <optional>

using namespace dagger;

namespace brawler {

	enum PlatformType 
	{
		EMPTY,
		BLOCK,
		ONEWAY,
		SPAWNER
	};

	using Tilemap = std::vector<std::vector<PlatformType>>;
	using TileCoords = std::pair<int, int>;

	class Level
	{
	public:
		static constexpr float TILE_WIDTH = 20.0f;
		static constexpr float TILE_HEIGHT = 20.0f;

		static unsigned LEVEL_WIDTH;
		static unsigned LEVEL_HEIGHT;

		static constexpr float DROPDOWN_OFFSET = 5.0f;

		static void Load(String name);

		static Entity Player1();
		static Entity Player2();
		
		static TileCoords WorldToTile(Vector2 worldPos);
		inline static int WorldToTileX(float x);
		inline static int WorldToTileY(float y);
		static Vector2 TileToWorld(int x, int y);
		inline static float TileToWorldX(int x);
		inline static float TileToWorldY(int y);

		static PlatformType getTile(int x, int y);
		static std::optional<float> getGround(Transform& t, Movable& m, SimpleCollision& c);
		static std::optional<float> getCeiling(Transform& t, Movable& m, SimpleCollision& c);
		static std::optional<float> getLeftWall(Transform& t, Movable& m, SimpleCollision& c);
		static std::optional<float> getRightWall(Transform& t, Movable& m, SimpleCollision& c);
	private:
		static Tilemap tiles;

		static Entity player1;
		static Entity player2;
	};

}
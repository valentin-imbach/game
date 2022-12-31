
#pragma once
#include "Tile.hpp"
#include "utils.hpp"

constexpr int MAP_WIDTH = 50;
constexpr int MAP_HEIGHT = 50;

class Map {
public:
	Map();
	void updateStyle(pair position, bool propagate = false);
	std::unique_ptr<Tile> tiles[MAP_WIDTH][MAP_HEIGHT];
	TileId::value getTileId(pair position);
private:
	uint seed;
};
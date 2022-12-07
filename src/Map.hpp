
#pragma once
#include "Tile.hpp"
#include "utils/utils.hpp"

constexpr int MAP_WIDTH = 500;
constexpr int MAP_HEIGHT = 500;

class Map {
public:
	Map();
	void updateStyle(pair position);
	std::unique_ptr<Tile> tiles[MAP_WIDTH][MAP_HEIGHT];
private:
	TileId getTileId(pair position);
	uint seed;
};

#pragma once
#include "Tile.hpp"
#include "utils/utils.hpp"

constexpr int MAP_WIDTH = 25;
constexpr int MAP_HEIGHT = 25;

class Map {
public:
	Map();
	void updateStyle(int x, int y);
	std::unique_ptr<Tile> tiles[MAP_WIDTH][MAP_HEIGHT];
};
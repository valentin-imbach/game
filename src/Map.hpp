
#pragma once
#include "Tile.hpp"
#include "utils/utils.hpp"

constexpr int MAP_WIDTH = 10;
constexpr int MAP_HEIGHT = 10;

class Map {
public:
	Map();
	void updateStyle(int x, int y);
	Tile tiles[MAP_WIDTH][MAP_HEIGHT];
};
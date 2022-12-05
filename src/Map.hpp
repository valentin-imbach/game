
#pragma once
#include "utils/utils.hpp"
#include "Tile.hpp"

constexpr int MAP_WIDTH = 10;
constexpr int MAP_HEIGHT = 10;

class Map {
public:
    Map();
    Tile tiles[MAP_WIDTH][MAP_HEIGHT];
};
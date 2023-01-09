
#pragma once
#include "Tile.hpp"
#include "utils.hpp"
#include "ResourceTemplates.hpp"
#include "Generation.hpp"

constexpr int MAP_WIDTH = 100;
constexpr int MAP_HEIGHT = 100;

class Map {
public:
	Map(uint seed);

	Biome::value getBiome(pair position);
	int getTemperature(pair position);
	int getPrecipitation(pair position);
	int getElevation(pair position);
	int getVegetation(pair position);
	int getVariation(pair position);

	void generate();

	void updateStyle(pair position, bool propagate = false);
	std::unique_ptr<Tile> tiles[MAP_WIDTH][MAP_HEIGHT];
	TileId::value getTileId(pair position);

	void analyse(int samples);

private:
	uint mapSeed;
	uint seed;
};
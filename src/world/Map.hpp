
#pragma once
#include "Tile.hpp"
#include "utils.hpp"
#include "ResourceTemplates.hpp"
#include "Generation.hpp"

class Map {
public:
	Map(pair size, uint seed);

	Biome::value getBiome(pair position);
	std::unique_ptr<Distribution> temparatureMap;
	std::unique_ptr<Distribution> precipitationMap;
	std::unique_ptr<Distribution> elevationMap;
	std::unique_ptr<Distribution> vegetationMap;
	std::unique_ptr<Distribution> variationMap;

	void generate();

	void updateStyle(pair position, bool propagate = false);
	std::vector<std::vector<std::unique_ptr<Tile>>> tiles;
	TileId::value getTileId(pair position);

	void analyse(int samples);

	pair size;

private:
	uint mapSeed;
	uint seed;
};
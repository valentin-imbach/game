
#pragma once
#include "Tile.hpp"
#include "utils.hpp"
#include "ResourceTemplates.hpp"
#include "Generation.hpp"

class Map {
public:
	Map(pair size, uint seed);
	Map(std::fstream& stream);

	Biome::value getBiome(pair position);
	std::unique_ptr<Distribution> temparatureMap;
	std::unique_ptr<Distribution> precipitationMap;
	std::unique_ptr<Distribution> elevationMap;
	std::unique_ptr<Distribution> vegetationMap;
	std::unique_ptr<Distribution> variationMap;

	void generate();
	void generateInterior();

	void updateStyle(pair position, bool propagate = false);
	std::vector<std::vector<std::unique_ptr<Tile>>> tiles;
	TileId::value getTileId(pair position);

	void analyse(int samples);
	bool inside(pair pos);

	pair size;

	void serialize(std::fstream& stream);
	SDL_Surface* makeMiniMap();

private:
	uint seed;
};
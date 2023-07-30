
#pragma once
#include "Tile.hpp"
#include "utils.hpp"
#include "ResourceTemplates.hpp"
#include "Environment.hpp"

class Map {
public:
	Map(pair size, uint seed);
	Map(std::fstream& stream);

	void generate(Environment* environment);
	void generateInterior(Environment* environment);

	void updateStyle(pair position, bool propagate = false);
	std::vector<std::vector<std::unique_ptr<Tile>>> tiles;
	TileId::value getTileId(pair position);

	// void analyse(int samples);
	bool inside(pair pos);

	pair size;

	void serialize(std::fstream& stream);
	SDL_Surface* makeMiniMap();

private:
	uint seed;
};

#pragma once
#include "Tile.hpp"
#include "utils.hpp"
#include "ResourceTemplates.hpp"
#include "Environment.hpp"

class Map {
public:
	Map(pair size, uint seed);
	Map(std::fstream& stream);

	void updateStyle(pair position, bool propagate = false);
	void updateStyle();
	
	std::vector<std::vector<std::unique_ptr<Tile>>> tiles;
	GroundId::value getGroundId(pair position);
	GroundId::value getWallId(pair position);

	// void analyse(int samples);
	bool inside(pair pos);

	pair size;

	void serialize(std::fstream& stream);
	SDL_Surface* makeMiniMap();

private:
	uint seed;
};
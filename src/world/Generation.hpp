
#pragma once
#include "Tile.hpp"
#include "ResourceTemplates.hpp"

ENUM(Biome,
OCEAN,
DESERT,
TUNDRA,
TAIGA,
SWAMP,
GRASSLAND,
FOREST,
SAVANNA,
RAINFOREST,
MOUNTAIN)

struct BiomeGroundTemplate {
	BiomeGroundTemplate(TileId::value tileId);
	TileId::value tileId;
	std::vector<ResourceId::value> resources;
};

class BiomeTemplate {
public:
	BiomeTemplate(std::string name);
	BiomeGroundTemplate* getGround(int variation);

	static std::array<std::unique_ptr<BiomeTemplate>, Biome::count> templates;
	static void setTemplates();
private:
	std::string name;
	std::vector<std::unique_ptr<BiomeGroundTemplate>> grounds;
};
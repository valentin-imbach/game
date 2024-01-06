
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
	GroundId::value groundId = GroundId::NONE;
	std::vector<std::pair<ResourceId::value, int>> resources;
};

struct BiomeTemplate {
	std::string name = "?";
	std::vector<BiomeGroundTemplate> grounds;

	BiomeGroundTemplate& getGround(int variation);

	static std::array<BiomeTemplate, Biome::count> templates;
	static void setTemplates();
};
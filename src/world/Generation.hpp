
#pragma once
#include "ResourceTemplates.hpp"

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
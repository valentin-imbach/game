
#pragma once
#include "utils.hpp"

struct WorldParameters {
	pair elevation = pair(-500, 1500);
	pair temperature = pair(-20, 40);
	pair percipitation = pair(0, 300);

	int period = 300;

	bool rivers = false;
	bool cliffs = false;
	bool structures = false;

	pair vegetation = pair(0, 100);
	pair variation = pair(0, 100);
	
	int fuzz = 0;
};

struct BiomeGroundTemplate {
	GroundId::value groundId = GroundId::NONE;
	std::vector<std::pair<ResourceId::value, int>> resources;
};

struct BiomeTemplate {
	std::string name = "?";
	std::vector<BiomeGroundTemplate> grounds;

	int temperature;
	int precipitation;
	int elevation;

	bool ocean;
	bool coast;

	BiomeGroundTemplate& getGround(int variation);

	static std::array<BiomeTemplate, Biome::count> templates;
	static void setTemplates(std::filesystem::path root);
};
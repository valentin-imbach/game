
#pragma once
#include "utils.hpp"
#include "Generation.hpp"

ENUM(WeatherId,
RAIN,
FOG,
SNOW)

class Environment {
public:
	Environment(uint seed);

	void draw(uint ticks);
	WeatherId::value weatherId;
	int intensity = 50;

	Biome::value getBiome(pair position);
	std::unique_ptr<Distribution> temparatureMap;
	std::unique_ptr<Distribution> precipitationMap;
	std::unique_ptr<Distribution> elevationMap;
	std::unique_ptr<Distribution> vegetationMap;
	std::unique_ptr<Distribution> variationMap;
private:
	uint seed;
};
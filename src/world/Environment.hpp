
#pragma once
#include "utils.hpp"
#include "Generation.hpp"

class Environment {
public:
	Environment(uint seed, WorldParameters params);

	void draw(uint ticks);
	WeatherId::value weatherId = WeatherId::NONE;
	int intensity = 50;

	Biome::value getBiome(pair position, bool ocean = false, bool coast = false);
	std::unique_ptr<Distribution<float>> temparatureMap;
	std::unique_ptr<Distribution<float>> precipitationMap;
	std::unique_ptr<Distribution<float>> elevationMap;
	std::unique_ptr<Distribution<float>> vegetationMap;
	std::unique_ptr<Distribution<float>> variationMap;

	std::unique_ptr<Distribution<vec>> fuzz;

	bool rivers;
	bool cliffs;
	bool structures;
private:
	uint seed;
};

#pragma once
#include "utils.hpp"

class Environment {
public:
	Environment(uint seed, RealmType::value realmType = RealmType::WORLD);

	void draw(uint ticks);
	WeatherId::value weatherId = WeatherId::NONE;
	int intensity = 50;

	Biome::value getBiome(pair position);
	std::unique_ptr<Distribution<float>> temparatureMap;
	std::unique_ptr<Distribution<float>> precipitationMap;
	std::unique_ptr<Distribution<float>> elevationMap;
	std::unique_ptr<Distribution<float>> vegetationMap;
	std::unique_ptr<Distribution<float>> variationMap;
private:
	uint seed;
	RealmType::value realmType;
};
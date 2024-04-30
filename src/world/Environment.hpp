
#pragma once
#include "utils.hpp"
#include "Generation.hpp"

ENUM(WeatherId,
CLEAR,
RAIN,
FOG,
SNOW)

ENUM(RealmType,
WORLD,
HOUSE,
CAVE,
DUNGEON)

class Environment {
public:
	Environment(uint seed, RealmType::value realmType = RealmType::WORLD);

	void draw(uint ticks);
	WeatherId::value weatherId = WeatherId::NONE;
	int intensity = 50;

	Biome::value getBiome(pair position);
	std::unique_ptr<Distribution> temparatureMap;
	std::unique_ptr<Distribution> precipitationMap;
	std::unique_ptr<Distribution> elevationMap;
	std::unique_ptr<Distribution> vegetationMap;
	std::unique_ptr<Distribution> variationMap;
private:
	uint seed;
	RealmType::value realmType;
};
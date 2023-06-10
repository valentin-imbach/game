
#include "Realm.hpp"
#include "EntityFactory.hpp"

Realm::Realm(pair size, uint seed) : size(size), seed(seed) {
	map = std::make_unique<Map>(size, seed);
}

void Realm::generate() {
	uint s = seed;
	for (int x = 0; x < map->size.x; x++) {
		for (int y = 0; y < map->size.y; y++) {
			pair position(x, y);
			Biome::value biome = map->getBiome(position);
			int variation = map->variationMap->get(position);
			int vegetation = map->vegetationMap->get(position);
			int choice = rand_int(s++, 0, 50 + vegetation);
			BiomeGroundTemplate* ground = BiomeTemplate::templates[biome]->getGround(variation);
			for (auto& p : ground->resources) {
				choice -= p.second;
				if (choice < 0) {
					EntityFactory::createResource(p.first, position);
					break;
				}
			}
		}
	}
}

bool Realm::free(pair anker, pair size = {1, 1}) {
	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			if (gridMap.find(anker + pair(x,y)) != gridMap.end()) return false;
		}
	}
	return true;
}

pair Realm::findFree(pair pos) {
	pair guess = pos;
	uint seet = 123;
	while (!free(guess)) {
		guess.x = rand_int(seed++, pos.x - 20, pos.x + 20);
		guess.y = rand_int(seed++, pos.y - 20, pos.y + 20);
	}
	return guess;
}
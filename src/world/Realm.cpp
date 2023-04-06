
#include "Realm.hpp"
#include "EntityFactory.hpp"

Realm::Realm(pair size, uint seed) : size(size), seed(seed) {
	map = std::make_unique<Map>(size, seed);
}

void Realm::generate() {
	uint seed = 456;
	for (int x = 0; x < map->size.x; x++) {
		for (int y = 0; y < map->size.y; y++) {
			pair position(x, y);
			Biome::value biome = map->getBiome(position);
			int variation = map->variationMap->get(position);
			int vegetation = map->vegetationMap->get(position);
			int choice = rand_int(seed++, 50 + vegetation);
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
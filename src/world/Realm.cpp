
#include "Realm.hpp"
#include "World.hpp"
#include "EntityFactory.hpp"

Realm::Realm(RealmId realmId, pair size, uint seed) : realmId(realmId), size(size), seed(seed) {
	map = std::make_unique<Map>(size, seed);
}

Realm::Realm(std::fstream& stream) {
	deserialise_object(stream, realmId);
	deserialise_object(stream, size);
	deserialise_object(stream, seed);
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
			int choice = noise::Int(s++, 0, 50 + vegetation);
			BiomeGroundTemplate* ground = BiomeTemplate::templates[biome]->getGround(variation);
			for (auto& p : ground->resources) {
				choice -= p.second;
				if (choice < 0) {
					Entity resource = EntityFactory::createResource(p.first, this, position);
					break;
				}
			}
		}
	}
}

void Realm::linkGrid(Entity entity) {
	world->gridSystem->link(entity, gridMap, solidMap, opaqueMap);
}

void Realm::unlinkGrid(Entity entity) {
	world->gridSystem->unlink(entity, gridMap, solidMap, opaqueMap);
}

void Realm::linkChunk(Entity entity) {
	world->chunkSystem->link(entity, chunks);
}

void Realm::unlinkChunk(Entity entity) {
	world->chunkSystem->unlink(entity, chunks);
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
		guess.x = noise::Int(seed++, pos.x - 20, pos.x + 20);
		guess.y = noise::Int(seed++, pos.y - 20, pos.y + 20);
	}
	return guess;
}

void Realm::serialise(std::fstream& stream) {
	serialise_object(stream, realmId);
	serialise_object(stream, size);
	serialise_object(stream, seed);
}
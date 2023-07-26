
#include "Realm.hpp"
#include "World.hpp"
#include "EntityFactory.hpp"

Realm::Realm(RealmId realmId, pair size, uint seed) : realmId(realmId), size(size), seed(seed) {
	map = std::make_unique<Map>(size, seed + 1);
	map->generate();
}

Realm::Realm(std::fstream& stream) {
	deserialise_object(stream, realmId);
	deserialise_object(stream, size);
	deserialise_object(stream, seed);
	map = std::make_unique<Map>(stream);
	//map->generate();
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

void Realm::linkGrid(Entity entity, pair anker, pair size, bool solid, bool opaque) {
	assert(entity);
	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			pair key = anker + pair(x, y);
			if (gridMap.find(key) != gridMap.end()) {
				WARNING("Trying to link more than one entity to tile");
				continue;
			}
			gridMap[key] = entity;
			if (solid) solidMap.insert(key);
			if (opaque) opaqueMap.insert(key);
		}
	}
}

void Realm::unlinkGrid(Entity entity, pair anker, pair size, bool solid, bool opaque) {
	assert(entity);
	for (int x = 0; x <size.x; x++) {
		for (int y = 0; y <size.y; y++) {
			pair key = anker + pair(x, y);
			if (gridMap.find(key) == gridMap.end()) {
				WARNING("Trying to remove non-existing grid link");
				continue;
			}
			gridMap.erase(key);
			if (solid) solidMap.erase(key);
			if (opaque) opaqueMap.erase(key);
		}
	}
}

void Realm::linkChunk(Entity entity, pair chunk) {
	assert(entity);
	if (chunks[chunk].find(entity) != chunks[chunk].end()) {
		WARNING("Trying to link entity", entity, "to chunk", chunk, "twice");
		return;
	}
	chunks[chunk].insert(entity);
}

void Realm::unlinkChunk(Entity entity, pair chunk) {
	assert(entity);
	if (chunks[chunk].find(entity) == chunks[chunk].end()) {
		WARNING("Trying to unlink non-existing entity", entity, "from chunk", chunk);
		return;
	}
	chunks[chunk].erase(entity);
}

bool Realm::free(pair anker, pair size = {1, 1}) {
	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			if (gridMap.find(anker + pair(x,y)) != gridMap.end()) return false;
		}
	}
	return true;
}

bool Realm::walkable(pair position) {
	if (!map->inside(position)) return false;
	if (map->getTileId(position) == TileId::WATER) return false;
	if (solidMap.find(position) != solidMap.end()) return false;
	return true;
}

bool Realm::opaque(pair position) {
	return opaqueMap.find(position) != opaqueMap.end();
}

pair Realm::findFree(pair pos, int radius, bool origin) {
	pair guess = pos;
	uint s = 123;
	while (!free(guess) || (!origin && guess == pos)) {
		guess.x = noise::Int(s++, pos.x - radius, pos.x + radius);
		guess.y = noise::Int(s++, pos.y - radius, pos.y + radius);
	}
	return guess;
}

void Realm::serialise(std::fstream& stream) {
	serialise_object(stream, realmId);
	serialise_object(stream, size);
	serialise_object(stream, seed);
	map->serialize(stream);
}
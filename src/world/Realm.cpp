
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

void Realm::linkGrid(Entity entity, GridComponent& gridComponent) {
	if (!entity) return;
	for (int x = 0; x < gridComponent.size.x; x++) {
		for (int y = 0; y < gridComponent.size.y; y++) {
			pair key = gridComponent.anker + pair(x, y);
			if (gridMap.find(key) != gridMap.end()) {
				WARNING("Trying to link more than one entity to a tile");
				continue;
			}
			gridMap[key] = entity;
			if (gridComponent.solid) solidMap.insert(key);
			if (gridComponent.opaque) opaqueMap.insert(key);
		}
	}
	//world->gridSystem->link(entity, gridMap, solidMap, opaqueMap);
}

void Realm::unlinkGrid(Entity entity, GridComponent& gridComponent) {
	if (!entity) return;
	for (int x = 0; x < gridComponent.size.x; x++) {
		for (int y = 0; y < gridComponent.size.y; y++) {
			pair key = gridComponent.anker + pair(x, y);
			if (gridMap.find(key) == gridMap.end()) {
				WARNING("Trying to remove non-existing link");
				continue;
			}
			gridMap.erase(key);
			if (gridComponent.solid) solidMap.erase(key);
			if (gridComponent.opaque) opaqueMap.erase(key);
		}
	}
}

void Realm::linkChunk(Entity entity, PositionComponent& positionComponent) {
	pair chunk = vec::round(positionComponent.position / CHUNK_SIZE);
	positionComponent.chunk = chunk;
	if (chunks[chunk].find(entity) != chunks[chunk].end()) {
		WARNING("Trying to link entity", entity, "to chunk", chunk, "twice");
		return;
	}
	chunks[chunk].insert(entity);
}

void Realm::unlinkChunk(Entity entity, PositionComponent& positionComponent) {
	if (chunks[positionComponent.chunk].find(entity) == chunks[positionComponent.chunk].end()) {
		WARNING("Trying to unlink non-existing entity", entity, "from chunk", positionComponent.chunk);
		return;
	}
	chunks[positionComponent.chunk].erase(entity);
}

bool Realm::free(pair anker, pair size = {1, 1}) {
	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			if (gridMap.find(anker + pair(x,y)) != gridMap.end()) return false;
		}
	}
	return true;
}

bool Realm::walkable(pair pos) {
	if (solidMap.find(pos) != solidMap.end()) return false;
	return map->getTileId(pos) != TileId::WATER;
}

pair Realm::findFree(pair pos, int radius, bool origin) {
	pair guess = pos;
	uint seet = 123;
	while (!free(guess) || (!origin && guess == pos)) {
		guess.x = noise::Int(seed++, pos.x - radius, pos.x + radius);
		guess.y = noise::Int(seed++, pos.y - radius, pos.y + radius);
	}
	return guess;
}

void Realm::serialise(std::fstream& stream) {
	serialise_object(stream, realmId);
	serialise_object(stream, size);
	serialise_object(stream, seed);
}
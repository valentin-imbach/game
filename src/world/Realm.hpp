
#pragma once

#include "utils.hpp"
#include "ECS_types.hpp"
#include "Map.hpp"
#include "Environment.hpp"
#include "Chunk.hpp"
#include "ChunkManager.hpp"
#include "Minimap.hpp"

class SDL_Texture;

#define CHUNK_REACH 10

class Realm {
public:
	Realm(RealmId realmId, uint seed);
	Realm(std::fstream& stream);

	void generate(RealmType::value realmType);

	void generateWorld(pair size);
	void generateHouse(pair size);
	void generateCave(int count, int length);
	void generateFlat(pair size);
	void generateDungeon();

	ChunkManager chunkManager;

	pair spawn;

	RealmId realmId;
	MapData mapData;

	std::unique_ptr<Map> map;
	std::unique_ptr<Environment> environment;
	std::unordered_map<pair, EntitySet> chunkEntities;
	GridMap gridMap;

	void linkGrid(Entity entity, pair anker, pair size, bool solid, bool opaque);
	void unlinkGrid(Entity entity, pair anker, pair size, bool solid, bool opaque);
	void linkChunk(Entity entity, pair chunk);
	void unlinkChunk(Entity entity, pair chunk);
	
	// void link(Entity entity);
	// void unlink(Entity entity);

	bool free(pair anker, pair size = {1, 1});
	bool walkable(pair pos);
	bool opaque(pair pos);

	pair findFree(pair pos, int radius = 5, bool origin = true);

	void serialise(std::fstream& stream);

private:
	uint seed;

	std::unordered_set<pair> solidMap;
	std::unordered_set<pair> opaqueMap;
};
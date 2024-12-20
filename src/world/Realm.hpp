
#pragma once

#include "utils.hpp"
#include "ECS_types.hpp"
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

	void generate(WorldParameters params);
	void generateHouse();
	// void generateCave(int count, int length);
	void generateFlat();
	void generateDungeon();
	void generateDungeon2();

	ChunkManager chunkManager;

	pair spawn;

	RealmId realmId;

	std::unique_ptr<Environment> environment;
	// std::unordered_map<pair, EntitySet> chunkEntities;
	GridMap gridMap;

	Minimap minimap;

	void link(Entity entity, pair anker, pair size, bool solid, bool opaque);
	void unlink(Entity entity, pair anker, pair size);

	void attach(Entity entity, pair chunk);
	void detach(Entity entity);

	bool free(pair anker, pair size = pair(1, 1));
	bool walkable(pair pos);
	bool opaque(pair pos);

	GroundId::value ground(pair position);

	pair findFree(pair pos, int radius = 5, bool origin = true);

	void serialise(std::fstream& stream);
	void serialise2(std::filesystem::path path);

private:
	uint seed;

	std::unordered_set<pair> solidMap;
	std::unordered_set<pair> opaqueMap;
};

#pragma once
#include "Chunk.hpp"
#include "Environment.hpp"

#define CHUNK_SIZE 16
#define CHUNK_REACH 10

class World;
using RealmId = unsigned char;
struct GridComponent;
struct PositionComponent;

ENUM(RealmType,
WORLD,
HOUSE)

class Realm {
public:
	Realm(RealmId realmId, pair size, uint seed, RealmType::value realmType);
	Realm(std::fstream& stream);

	void generate();

	// std::unordered_map<pair, Chunk> chunks;

	RealmId realmId;
	SDL_Texture* minimap;

	std::unique_ptr<Map> map;
	std::unique_ptr<Environment> environment;
	std::unordered_map<pair, EntitySet> chunks;
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
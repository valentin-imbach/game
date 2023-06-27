
#pragma once
#include "Chunk.hpp"

class World;
using RealmId = unsigned char;
struct GridComponent;
struct PositionComponent;

class Realm {
public:
	Realm(RealmId realmId, pair size, uint seed);
	Realm(std::fstream& stream);

	void generate();

	// void serialise(std::fstream& stream);
	// void deserialise(std::fstream& stream);

	// std::unordered_map<pair, Chunk> chunks;

	RealmId realmId;

	pair size;
	std::unique_ptr<Map> map;
	GridMap gridMap;
	std::unordered_set<pair> solidMap;
	std::unordered_set<pair> opaqueMap;

	void linkGrid(Entity entity, GridComponent& gridComponent);
	void unlinkGrid(Entity entity, GridComponent& gridComponent);
	void linkChunk(Entity entity, PositionComponent& positionComponent);
	void unlinkChunk(Entity entity, PositionComponent& positionComponent);
	
	// void unlink(Entity entity);

	bool free(pair anker, pair size);
	pair findFree(pair pos);

	void serialise(std::fstream& stream);

private:
	std::unordered_map<pair, EntitySet> chunks;
	uint seed;
	
	friend class World;
};
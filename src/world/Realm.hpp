
#pragma once
#include "Chunk.hpp"

class World;

class Realm {
public:
	Realm(World* world, pair size, uint seed);

	void generate();

	// void serialise(std::fstream& stream);
	// void deserialise(std::fstream& stream);

	// std::unordered_map<pair, Chunk> chunks;

	pair size;
	std::unique_ptr<Map> map;
	GridMap gridMap;
	std::unordered_set<pair> solidMap;
	std::unordered_set<pair> opaqueMap;
	void linkGrid(Entity entity);
	void unlinkGrid(Entity entity);
	void linkChunk(Entity entity);
	void unlinkChunk(Entity entity);
	
	// void unlink(Entity entity);

	bool free(pair anker, pair size);
	pair findFree(pair pos);

private:
	std::unordered_map<pair, EntitySet> chunks;
	World* world;
	uint seed;
	
	friend class World;
};
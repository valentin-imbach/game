
#pragma once
#include "Chunk.hpp"

class Realm {
public:
	Realm(pair size, uint seed);

	void generate();

	// void serialise(std::fstream& stream);
	// void deserialise(std::fstream& stream);

	// std::unordered_map<pair, Chunk> chunks;

	pair size;
	std::unique_ptr<Map> map;
	GridMap gridMap;
	std::unordered_set<pair> solidMap;
	std::unordered_set<pair> opaqueMap;

	bool free(pair anker, pair size);
	pair findFree(pair pos);

private:
	uint seed;

};
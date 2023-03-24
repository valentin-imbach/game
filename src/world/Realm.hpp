
#pragma once
#include "Chunk.hpp"

class Realm {
public:
	Realm();

	void update(uint dt);
	void draw();

	void serialise(std::fstream& stream);
	void deserialise(std::fstream& stream);

	std::unordered_map<pair, Chunk> chunks;

};
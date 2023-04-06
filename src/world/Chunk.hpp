
#pragma once
#include "ECS.hpp"
#include "Map.hpp"

class Chunk {
public:
	Chunk();

	void serialise(std::fstream& stream);
	void deserialise(std::fstream& stream);

	Map map;
	pair position;
};

#pragma once
#include "ECS.hpp"
#include "Map.hpp"

class Chunk {
public:
	Chunk();

	void serialise(std::fstream& stream);
	void deserialise(std::fstream& stream);

	void update(uint dt);
	void draw();

	ECS ecs;
	Map map;
};
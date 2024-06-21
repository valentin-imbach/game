
#pragma once
#include "utils.hpp"
#include "Chunk.hpp"
#include "Camera.hpp"

class Realm;

class ChunkManager {
public:
	ChunkManager(uint seed);
	void generateChunk(pair position, ChunkStage::value target, Environment* environment);
	void draw(Camera camera, uint ticks);

	void updateStyle(pair position, bool propagate = false);

	std::pair<SDL_Surface*, pair> minimap();

	std::array<ushort, ChunkStage::count> stageBuffer = {};

	Realm* realm;

private:
	uint seed;
	pair getChunk(pair position);
	pair getOffset(pair position);

	void setGround(pair chunk);
	void setTiles(pair chunk);

	GroundId::value getGround(pair position);
	bool checkStage(pair chunk, ChunkStage::value stage = ChunkStage::NONE);
	std::unordered_map<pair, Chunk> chunks;

	PerlinDistortion pdist = PerlinDistortion(noise::UInt(seed + 4));

	friend class Chunk;
};
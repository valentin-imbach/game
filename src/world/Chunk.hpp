
#pragma once
#include "ECS.hpp"
#include "Map.hpp"
#include "Camera.hpp"
#include "definitions.hpp"

#define CHUNK_SIZE 16

class ChunkManager;

ENUM(ChunkStage,
NODE,
BIOME,
RIVER,
GROUND,
TILES,
OBJECTS,
LOADED)

class Chunk {
public:
	Chunk(pair position);

	void serialise(std::fstream& stream);
	void serialise2(std::filesystem::path path);
	void deserialise(std::fstream& stream);

	ChunkStage::value stage = ChunkStage::NONE;

	void advance(ChunkManager* manager, Environment* environemnt);

	bool inside(pair pos);

	void drawTiles(Camera camera, uint ticks);

	// Map map;

private:
	void setNode(ChunkManager* manager, Environment* environemnt);
	void setBiome(ChunkManager* manager, Environment* environemnt);
	void setRiver(ChunkManager* manager, Environment* environemnt);
	void setGround(ChunkManager* manager, Environment* environment);
	void setTiles(ChunkManager* manager);
	void setObjects(ChunkManager* manager, Environment* environment);

	void refreshMap();

	pair node;
	pair nodeOffset;
	Biome::value biome;

	Tile tiles[CHUNK_SIZE][CHUNK_SIZE] = {};
	Entity entityGrid[CHUNK_SIZE][CHUNK_SIZE] = {};
	bool solid[CHUNK_SIZE][CHUNK_SIZE] = {};
	bool opaque[CHUNK_SIZE][CHUNK_SIZE] = {};

	uint seed;
	pair position;

	Direction::value down = Direction::NONE;
	Direction::value up = Direction::NONE;

	float elevation;

	ushort river = 0;
	bool riverSource = false;

	Entity grid[CHUNK_SIZE][CHUNK_SIZE] = {};

	SDL_Texture* mapTexture = nullptr;

	friend class World;
	friend class ChunkManager;
	friend class Minimap;
};
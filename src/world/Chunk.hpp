
#pragma once
#include "ECS.hpp"
#include "Tile.hpp"
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

private:
	void setNode(ChunkManager* manager, Environment* environemnt);
	void setBiome(ChunkManager* manager, Environment* environemnt);
	void setRiver(ChunkManager* manager, Environment* environemnt);
	void setGround(ChunkManager* manager, Environment* environment);
	void setTiles(ChunkManager* manager);
	void setObjects(ChunkManager* manager, Environment* environment);

	void refreshMap(Environment* environemnt);

	pair node;
	pair nodeOffset;
	Biome::value biome;

	Tile tiles[CHUNK_SIZE][CHUNK_SIZE] = {};
	bool solid[CHUNK_SIZE][CHUNK_SIZE] = {};
	bool opaque[CHUNK_SIZE][CHUNK_SIZE] = {};

	Entity entityGrid[CHUNK_SIZE][CHUNK_SIZE] = {};
	EntitySet entities = {};

	uint seed;
	pair position;

	Direction::value down = Direction::NONE;
	Direction::value up = Direction::NONE;

	float elevation;

	bool cliff;

	bool ocean;
	bool coastal;

	ushort river = 0;
	bool riverSource = false;

	Entity grid[CHUNK_SIZE][CHUNK_SIZE] = {};

	SDL_Texture* mapTexture = nullptr;
	SDL_Texture* tempTexture = nullptr;

	friend class World;
	friend class ChunkManager;
	friend class Minimap;
	friend class DebugScreen;
	friend class Realm;
};
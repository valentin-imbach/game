
#pragma once
#include "utils.hpp"
#include "Chunk.hpp"
#include "Camera.hpp"
class World;

class Realm;

class ChunkManager {
public:
	ChunkManager(uint seed);
	void generateChunk(pair position, ChunkStage::value target, Environment* environment);
	void draw(Camera camera, uint ticks);

	void updateStyle(pair position, bool propagate = false);

	void serialise2(std::filesystem::path path);

	GroundId::value getGround(pair position);
	bool opaque(pair position);
	bool solid(pair position);
	Entity gridEntity(pair position);

	void setGround(pair position, GroundId::value groundId);

	void link(pair position, Entity entity, bool solid, bool opaque);
	void unlink(pair position, Entity entity);

	void attach(Entity entity, pair chunk);
	void detach(Entity entity);

	std::array<ushort, ChunkStage::count> stageBuffer = {};

	Realm* realm;

	pair getChunk(vec position);

private:
	uint seed;

	pair getOffset(pair position);

	bool checkStage(pair chunk, ChunkStage::value stage = ChunkStage::NONE);
	std::unordered_map<pair, Chunk> chunks;
	std::unordered_map<Entity, pair> entityIndex;

	void reballance(World* world);

	friend class Chunk;
	friend class Minimap;
	friend class World;
	friend class Realm;
	friend class DebugScreen;
};
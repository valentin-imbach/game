
#pragma once
#include "ECS_types.hpp"
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

#define CHUNK_SIZE 16
#define CHUNK_REACH 10

class ChunkSystem : public System {
public:
	void update(std::unordered_map<pair, std::set<Entity>>& chunks) {
		for (Entity entity : entities) assign(entity, chunks);
	}

	void assign(Entity entity, std::unordered_map<pair, std::set<Entity>>& chunks) {
		PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
		vec offset = positionComponent.position - CHUNK_SIZE * positionComponent.chunk;

		if (abs(offset.x) >= CHUNK_REACH || abs(offset.y) >= CHUNK_REACH) {
			pair newChunk = round(positionComponent.position/CHUNK_SIZE);
			chunks[positionComponent.chunk].erase(entity);
			chunks[newChunk].insert(entity);
			positionComponent.chunk = newChunk;
		}
	}
};

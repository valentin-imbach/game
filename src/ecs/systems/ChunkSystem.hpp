
#pragma once
#include "Components.hpp"
#include "ECS.hpp"
#include "ECS_types.hpp"
#include "System.hpp"

#define CHUNK_SIZE 16
#define CHUNK_REACH 10

class ChunkSystem : public System {
public:
	void update(RealmManager& realmManager) {
		for (Entity entity : entities) reassign(entity, realmManager);
	}

	void link(Entity entity, std::unordered_map<pair, EntitySet>& chunks) {
		PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
		pair chunk = vec::round(positionComponent.position / CHUNK_SIZE);
		positionComponent.chunk = chunk;
		if (chunks[chunk].find(entity) != chunks[chunk].end()) {
			WARNING("Trying to link entity to chunk twice");
			return;
		}
		chunks[chunk].insert(entity);
	}

	void unlink(Entity entity, std::unordered_map<pair, EntitySet>& chunks) {
		PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
		if (chunks[positionComponent.chunk].find(entity) == chunks[positionComponent.chunk].end()) {
			WARNING("Trying to unlink non-existing entity from chunk");
			return;
		}
		chunks[positionComponent.chunk].erase(entity);
	}

	void reassign(Entity entity, RealmManager& realmManager) {
		PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
		Realm* realm = realmManager.getRealm(positionComponent.realmId);

		vec offset = positionComponent.position - CHUNK_SIZE * positionComponent.chunk;

		if (std::abs(offset.x) >= CHUNK_REACH || std::abs(offset.y) >= CHUNK_REACH) {
			pair newChunk = vec::round(positionComponent.position / CHUNK_SIZE);
			realm->unlinkChunk(entity);
			realm->linkChunk(entity);
		}
	}
};

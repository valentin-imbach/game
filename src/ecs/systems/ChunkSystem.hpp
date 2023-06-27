
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

	void reassign(Entity entity, RealmManager& realmManager) {
		PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
		Realm* realm = realmManager.getRealm(positionComponent.realmId);

		vec offset = positionComponent.position - CHUNK_SIZE * positionComponent.chunk;

		if (std::abs(offset.x) >= CHUNK_REACH || std::abs(offset.y) >= CHUNK_REACH) {
			pair newChunk = vec::round(positionComponent.position / CHUNK_SIZE);
			realm->unlinkChunk(entity, positionComponent);
			realm->linkChunk(entity, positionComponent);
		}
	}
};

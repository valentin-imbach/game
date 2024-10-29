
#pragma once
#include "Components.hpp"
#include "ECS.hpp"
#include "ECS_types.hpp"
#include "System.hpp"

class ChunkSystem : public System {
public:
	void update(RealmManager& realmManager) {
		for (Entity entity : entities) reassign(entity, realmManager);
	}

	void reassign(Entity entity, RealmManager& realmManager) {
		PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
		vec offset = positionComponent.position - CHUNK_SIZE * positionComponent.chunk;
		if (std::abs(offset.x) >= CHUNK_REACH || std::abs(offset.y) >= CHUNK_REACH) {
			Realm* realm = realmManager.getRealm(positionComponent.realmId);
			realm->unlinkChunk(entity, positionComponent.chunk);
			positionComponent.chunk = realm->chunkManager.getChunk(positionComponent.position);
			realm->linkChunk(entity, positionComponent.chunk);
		}
	}
};

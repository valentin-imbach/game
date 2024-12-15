
#pragma once
#include "Components.hpp"
#include "ECS.hpp"
#include "System.hpp"
#include "utils.hpp"

class PositionSystem : public System {

	

	void rebuild(RealmManager& realmManager) {
		for (Entity entity : entities) {
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			Realm* realm = realmManager.getRealm(positionComponent.realmId);
			pair chunk = realm->chunkManager.getChunk(positionComponent.position);
			realm->attach(entity, chunk);
		}
	}
};
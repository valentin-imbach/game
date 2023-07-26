
#pragma once
#include "System.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class DeathSystem : public System {
public:
	void update(RealmManager& realmManager) {
		EntitySet copy(entities);
		for (Entity entity : copy) {
			if (ecs->hasComponent<PositionComponent>(entity)) {
				PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
				Realm* realm = realmManager.getRealm(positionComponent.realmId);
				realm->unlinkChunk(entity, positionComponent.chunk);
			}

			if (ecs->hasComponent<GridComponent>(entity)) {
				GridComponent& gridComponent = ecs->getComponent<GridComponent>(entity);
				PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
				Realm* realm = realmManager.getRealm(positionComponent.realmId);
				realm->unlinkGrid(entity, gridComponent.anker, gridComponent.size, gridComponent.solid, gridComponent.opaque);
			}
			ecs -> destroyEntity(entity);
		}
	}
};
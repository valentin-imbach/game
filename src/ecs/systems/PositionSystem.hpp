
#pragma once
#include "Components.hpp"
#include "ECS.hpp"
#include "System.hpp"
#include "utils.hpp"

class PositionSystem : public System {
public:
	void rebuild(RealmManager& realmManager) {
		for (Entity entity : entities) {
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			Realm* realm = realmManager.getRealm(positionComponent.realmId);
			realm->linkChunk(entity, positionComponent.chunk);
		}
	}
};
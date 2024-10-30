
#pragma once
#include "Components.hpp"
#include "ECS.hpp"
#include "System.hpp"
#include "utils.hpp"

class GridSystem : public System {
public:
	void rebuild(RealmManager& realmManager) {
		for (Entity entity : entities) {
			GridComponent& gridComponent = ecs->getComponent<GridComponent>(entity);
			Realm* realm = realmManager.getRealm(gridComponent.realmId);
			realm->link(entity, gridComponent.anker, gridComponent.size, gridComponent.solid, gridComponent.opaque);
		}
	}
};
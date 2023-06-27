
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
			Realm* realm = realmManager.getRealm(ecs->getComponent<GridComponent>(entity).realmId);
			realm->linkGrid(entity, gridComponent);
		}
	}
};
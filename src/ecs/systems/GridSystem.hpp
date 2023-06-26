
#pragma once
#include "Components.hpp"
#include "ECS.hpp"
#include "System.hpp"
#include "utils.hpp"

class GridSystem : public System {
public:
	void link(Entity entity, GridMap& gridMap, std::unordered_set<pair>& solidMap, std::unordered_set<pair>& opaqueMap) {
		if (!entity) return;
		GridComponent& gridComponent = ecs->getComponent<GridComponent>(entity);
		for (int x = 0; x < gridComponent.size.x; x++) {
			for (int y = 0; y < gridComponent.size.y; y++) {
				pair key = gridComponent.anker + pair(x, y);
				if (gridMap.find(key) != gridMap.end()) {
					WARNING("Trying to link more than one entity to a tile");
					continue;
				}
				gridMap[key] = entity;
				if (gridComponent.solid) solidMap.insert(key);
				if (gridComponent.opaque) opaqueMap.insert(key);
			}
		}
	}

	void unlink(Entity entity, GridMap& gridMap, std::unordered_set<pair>& solidMap, std::unordered_set<pair>& opaqueMap) {
		if (!entity) return;
		GridComponent& gridComponent = ecs->getComponent<GridComponent>(entity);
		for (int x = 0; x < gridComponent.size.x; x++) {
			for (int y = 0; y < gridComponent.size.y; y++) {
				pair key = gridComponent.anker + pair(x, y);
				if (gridMap.find(key) == gridMap.end()) {
					WARNING("Trying to remove non-existing link");
					continue;
				}
				gridMap.erase(key);
				if (gridComponent.solid) solidMap.erase(key);
				if (gridComponent.opaque) opaqueMap.erase(key);
			}
		}
	}

	void rebuild(RealmManager& realmManager) {
		for (Entity entity : entities) {
			Realm* realm = realmManager.getRealm(ecs->getComponent<GridComponent>(entity).realmId);
			link(entity, realm->gridMap, realm->solidMap, realm->opaqueMap);
		}
	}
};
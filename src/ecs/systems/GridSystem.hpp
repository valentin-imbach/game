
#pragma once
#include "Components.hpp"
#include "ECS.hpp"
#include "System.hpp"
#include "utils.hpp"

class GridSystem : public System {
public:
	void update(GridMap& gridMap, std::unordered_set<pair>& solidMap) {
		for (Entity entity : entities) {
			GridComponent& gridComponent = ecs->getComponent<GridComponent>(entity);
			if (ecs->hasComponent<DeathComponent>(entity)) { //TODO: Move to own system
				for (int x = 0; x < gridComponent.size.x; x++) {
					for (int y = 0; y < gridComponent.size.y; y++) {
						gridMap.erase(gridComponent.anker + pair(x, y));
						solidMap.erase(gridComponent.anker + pair(x, y));
					}
				}
			}
			if (!gridComponent.linked) {
				for (int x = 0; x < gridComponent.size.x; x++) {
					for (int y = 0; y < gridComponent.size.y; y++) {
						gridMap[gridComponent.anker + pair(x, y)] = entity;
						if (gridComponent.solid) solidMap.insert(gridComponent.anker + pair(x, y));
					}
				}
				gridComponent.linked = true;
			}
		}
	}
	void rebuild(GridMap& gridMap, std::unordered_set<pair>& solidMap) {
		gridMap.clear();
		for (Entity entity : entities) {
			GridComponent& gridComponent = ecs->getComponent<GridComponent>(entity);
			for (int x = 0; x < gridComponent.size.x; x++) {
				for (int y = 0; y < gridComponent.size.y; y++) {
					gridMap[gridComponent.anker + pair(x, y)] = entity;
					if (gridComponent.solid) solidMap.insert(gridComponent.anker + pair(x, y));
				}
			}
		}
	}
};
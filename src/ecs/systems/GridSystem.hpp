
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
						pair offset(x, y);
						gridMap.erase(gridComponent.anker + offset);
					}
				}
			}
			if (!gridComponent.linked) {
				for (int x = 0; x < gridComponent.size.x; x++) {
					for (int y = 0; y < gridComponent.size.y; y++) {
						pair offset(x, y);
						gridMap[gridComponent.anker + offset] = entity;
						if (gridComponent.solid) solidMap.insert(gridComponent.anker + offset);
					}
				}
				gridComponent.linked = true;
			}
		}
	}
	void rebuild(GridMap& gridMap) {
		gridMap.clear();
		for (Entity entity : entities) {
			pair anker = ecs->getComponent<GridComponent>(entity).anker;
			pair size = ecs->getComponent<GridComponent>(entity).size;
			for (int x = 0; x < size.x; x++) {
				for (int y = 0; y < size.y; y++) {
					pair offset(x, y);
					gridMap[anker + offset] = entity;
				}
			}
		}
	}
};

#pragma once
#include "Components.hpp"
#include "ECS.hpp"
#include "System.hpp"
#include "utils.hpp"

class GridDeathSystem : public System {
public:
	void update(GridMap& gridMap, std::unordered_set<pair>& solidMap) {
		for (Entity entity : entities) {
			GridComponent& gridComponent = ecs->getComponent<GridComponent>(entity);
			for (int x = 0; x < gridComponent.size.x; x++) {
				for (int y = 0; y < gridComponent.size.y; y++) {
					gridMap.erase(gridComponent.anker + pair(x, y));
					solidMap.erase(gridComponent.anker + pair(x, y));
				}
			}
		}
	}
};
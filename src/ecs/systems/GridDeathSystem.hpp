
#pragma once
#include "Components.hpp"
#include "ECS.hpp"
#include "System.hpp"
#include "utils.hpp"

class GridDeathSystem : public System {
public:
	void update(GridMap& gridMap, std::unordered_set<pair>& solidMap, std::unordered_set<pair>& opaqueMap) {
		for (Entity entity : entities) {
			GridComponent& gridComponent = ecs->getComponent<GridComponent>(entity);
			for (int x = 0; x < gridComponent.size.x; x++) {
				for (int y = 0; y < gridComponent.size.y; y++) {
					pair pos = gridComponent.anker + pair(x, y);
					gridMap.erase(pos);
					solidMap.erase(pos);
					opaqueMap.erase(pos);
				}
			}
		}
	}
};
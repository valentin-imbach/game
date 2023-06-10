
#pragma once
#include "Components.hpp"
#include "ECS.hpp"
#include "System.hpp"
#include "utils.hpp"

class GridSystem : public System {
public:

	void link(GridMap& gridMap, std::unordered_set<pair>& solidMap, std::unordered_set<pair>& opaqueMap, Entity entity) {
		if (!entity) return;
		GridComponent& gridComponent = ecs->getComponent<GridComponent>(entity);
		for (int x = 0; x < gridComponent.size.x; x++) {
			for (int y = 0; y < gridComponent.size.y; y++) {
				gridMap[gridComponent.anker + pair(x, y)] = entity;
				if (gridComponent.solid) solidMap.insert(gridComponent.anker + pair(x, y));
				if (gridComponent.opaque) opaqueMap.insert(gridComponent.anker + pair(x, y));
			}
		}
	}

	void rebuild(GridMap& gridMap, std::unordered_set<pair>& solidMap, std::unordered_set<pair>& opaqueMap) {
		gridMap.clear();
		solidMap.clear();
		opaqueMap.clear();
		for (Entity entity : entities) link(gridMap, solidMap, opaqueMap, entity);
	}
};
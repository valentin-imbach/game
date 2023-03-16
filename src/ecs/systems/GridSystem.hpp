
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "utils.hpp"

class GridSystem : public System {
public:
	void update(GridMap* gridMap) {
		for (Entity entity : entities) {
			if (ecs->hasComponent<DeathComponent>(entity)) {
				pair anker = ecs -> getComponent<GridComponent>(entity).anker;
				pair size = ecs -> getComponent<GridComponent>(entity).size;
				for (int x = 0; x < size.x; x++) {
					for (int y = 0; y < size.y; y++) {
						pair offset(x,y);
						gridMap->erase(anker + offset);
					}
				}
			}
		}
	}
	void rebuild(GridMap* gridMap) {
		gridMap -> clear();
		for (Entity entity : entities) {
			pair anker = ecs -> getComponent<GridComponent>(entity).anker;
			pair size = ecs -> getComponent<GridComponent>(entity).size;
			for (int x = 0; x < size.x; x++) {
				for (int y = 0; y < size.y; y++) {
					pair offset(x,y);
					(*gridMap)[anker + offset] = entity;
				}
			}
			
		}
	}
};
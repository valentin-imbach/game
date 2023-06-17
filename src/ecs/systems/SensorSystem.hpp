
#pragma once
#include "System.hpp"
#include "ECS.hpp"
#include "Components.hpp"
#include "pathfinding.hpp"

class SensorSystem : public System {
public:
	void update(std::unordered_set<pair>& opaqueMap, Entity player, uint ticks) {
		for (Entity entity : entities) {
			SensorComponent& sensorComponent = ecs->getComponent<SensorComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);

			if (!player) return;
			vec playerPosition = ecs->getComponent<PositionComponent>(player).position;

			float d = vec::dist(positionComponent.position, playerPosition);
			if (d > sensorComponent.radius) return;

			if (visible(positionComponent.position, playerPosition, opaqueMap).first) {
				sensorComponent.position = playerPosition;
				sensorComponent.lastSeen = ticks;
				sensorComponent.engaged = true;
			}
		}
	}
};
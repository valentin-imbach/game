
#pragma once
#include "System.hpp"
#include "ECS.hpp"
#include "Components.hpp"
#include "pathfinding.hpp"

class SensorSystem : public System {
public:
	void update(Entity player, uint ticks, RealmManager& realmManager) {
		for (Entity entity : entities) {
			SensorComponent& sensorComponent = ecs->getComponent<SensorComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);

			sensorComponent.engaged = false;
			if (!player) return;
			PositionComponent& playerPositionComponent = ecs->getComponent<PositionComponent>(player);
			if (playerPositionComponent.realmId != positionComponent.realmId) return;
			Realm* realm = realmManager.getRealm(positionComponent.realmId);

			float d = vec::dist(positionComponent.position, playerPositionComponent.position);
			if (d > sensorComponent.radius) return;

			auto lambda = [realm](pair pos){ return !realm->opaque(pos); };
			if (ai::visible(positionComponent.position, playerPositionComponent.position, lambda).first) {
				sensorComponent.position = playerPositionComponent.position;
				sensorComponent.lastSeen = ticks;
				sensorComponent.engaged = true;
			}
		}
	}
};
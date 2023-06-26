
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

			if (!player) return;
			PositionComponent& playerPositionComponent = ecs->getComponent<PositionComponent>(entity);
			if (playerPositionComponent.realmId != positionComponent.realmId) return;
			Realm* realm = realmManager.getRealm(positionComponent.realmId);

			float d = vec::dist(positionComponent.position, playerPositionComponent.position);
			if (d > sensorComponent.radius) return;

			if (ai::visible(positionComponent.position, playerPositionComponent.position, realm->opaqueMap).first) {
				sensorComponent.position = playerPositionComponent.position;
				sensorComponent.lastSeen = ticks;
				sensorComponent.engaged = true;
			}
		}
	}
};
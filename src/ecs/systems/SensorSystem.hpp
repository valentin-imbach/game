
#pragma once
#include "System.hpp"
#include "ECS.hpp"
#include "Components.hpp"
#include "pathfinding.hpp"

class SensorSystem : public System {

	ROSTER(SENSOR, POSITION)

	void update(EntitySet& ents, uint ticks, RealmManager& realmManager) {
		for (Entity entity : entities) {
			SensorComponent& sensorComponent = ecs->getComponent<SensorComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);

			sensorComponent.engaged = false;
			if (!sensorComponent.tag) return;

			for (Entity ent : ents) {
				if (!ecs->hasComponent<TagComponent>(ent)) continue;
				TagComponent& entTagComponent = ecs->getComponent<TagComponent>(ent);
				if (!entTagComponent.tags[sensorComponent.tag]) continue;

				if (!ecs->hasComponent<PositionComponent>(ent)) continue;
				PositionComponent& entPositionComponent = ecs->getComponent<PositionComponent>(ent);
				if (entPositionComponent.realmId != positionComponent.realmId) continue;

				float d = vec::dist(positionComponent.position, entPositionComponent.position);
				if (d > sensorComponent.radius) continue;

				Realm* realm = realmManager.getRealm(positionComponent.realmId);
				auto lambda = [realm](pair pos){ return !realm->opaque(pos); };
				if (!ai::visible(positionComponent.position, entPositionComponent.position, lambda).first) continue;

				sensorComponent.position = entPositionComponent.position;
				sensorComponent.lastSeen = ticks;
				sensorComponent.engaged = true;
				if (ecs->hasComponent<PlayerComponent>(ent)) {
					PlayerComponent& playerComponent = ecs->getComponent<PlayerComponent>(ent);
					InventoryComponent& inventoryComponent = ecs->getComponent<InventoryComponent>(ent);
					sensorComponent.item = inventoryComponent.inventory.itemContainers[playerComponent.activeSlot][0].item;
				} else {
					sensorComponent.item = 0;
				}

				break;
			}
		}
	}
};
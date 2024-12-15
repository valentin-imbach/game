
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "EntityFactory.hpp"

class AiMeleeSystem : public System {

	ROSTER(AI, AI_MELEE, SENSOR, POSITION, HAND, ACTION)

	void score(uint ticks) {
		for (Entity entity : entities) {
			AiComponent& aiComponent = ecs->getComponent<AiComponent>(entity);
			AiMeleeComponent& aiMeleeComponent = ecs->getComponent<AiMeleeComponent>(entity);
			SensorComponent& sensorComponent = ecs->getComponent<SensorComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			
			bool inRange = vec::dist(sensorComponent.position, positionComponent.position) < 1;
			bool able = ticks - aiMeleeComponent.lastHit > aiMeleeComponent.cooldown;

			if (sensorComponent.engaged && inRange && able) {
				aiComponent.scores[AiState::MELEE] = 90;
			} else {
				aiComponent.scores[AiState::MELEE] = 0;
			}
		}
	}

	void update(uint ticks, RealmManager& realmManager) {
		for (Entity entity : entities) {
			AiComponent& aiComponent = ecs->getComponent<AiComponent>(entity);
			if (aiComponent.state != AiState::MELEE) continue;
			
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			HandComponent& handComponent = ecs->getComponent<HandComponent>(entity);
			AiMeleeComponent& aiMeleeComponent = ecs->getComponent<AiMeleeComponent>(entity);
			SensorComponent& sensorComponent = ecs->getComponent<SensorComponent>(entity);
			ActionComponent& actionComponent = ecs->getComponent<ActionComponent>(entity);

			if (ticks - aiMeleeComponent.lastHit < aiMeleeComponent.cooldown) continue;

			Entity weapon = 0;
			if (ecs->hasComponent<InventoryComponent>(entity)) {
				weapon = ecs->getComponent<InventoryComponent>(entity).inventory.itemContainers[0][0].item;
			}
			handComponent.item = weapon;

			if (actionComponent.actionState == ActionState::IDLE) {
				actionComponent.actionState = ActionState::ATTACK;
				actionComponent.position = sensorComponent.position;
				actionComponent.start = ticks;
				actionComponent.trigger = ticks + 150;
				actionComponent.end = ticks + 300;
				actionComponent.item = weapon;
				aiMeleeComponent.lastHit = ticks;
			}
		}
	}
};
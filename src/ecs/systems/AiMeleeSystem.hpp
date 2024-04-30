
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "EntityFactory.hpp"

class AiMeleeSystem : public System {
public:
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
			AiMeleeComponent& aiMeleeComponent = ecs->getComponent<AiMeleeComponent>(entity);
			SensorComponent& sensorComponent = ecs->getComponent<SensorComponent>(entity);

			if (ticks - aiMeleeComponent.lastHit < aiMeleeComponent.cooldown) continue;
			Realm* realm = realmManager.getRealm(positionComponent.realmId);
			vec force = {0,0.1};
			if (vec::dist(sensorComponent.position, positionComponent.position) > 0.001f) {
				force = vec::normalise(sensorComponent.position - positionComponent.position) / 10;
			}
			EntityFactory::createDamageArea(realm, sensorComponent.position, vec(0.2f, 0.2f), ticks, 1, force, entity);
			aiMeleeComponent.lastHit = ticks;

		}
	}
};
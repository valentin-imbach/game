
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "EntityFactory.hpp"

class ExplosiveSystem : public System {

	ROSTER(EXPLOSIVE, POSITION, SPRITE)

	void update(uint ticks, Camera& camera) {
		for (Entity entity : entities) {
			ExplosiveComponent& explosiveComponent = ecs->getComponent<ExplosiveComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			SpriteComponent& spriteCmponent = ecs->getComponent<SpriteComponent>(entity);

			if (!explosiveComponent.trigger) return;

			if (explosiveComponent.trigger < ticks) {
				EntityFactory::createDamageArea(positionComponent.realmId, positionComponent.position, Shape(2.0f), ticks, 1, 50);
				camera.shake = ticks;
				ecs->addComponent<DeathComponent>({}, entity);
				return;
			}

			float left = explosiveComponent.trigger - ticks;
			float slope = 0.002f*left - 5000.0f/(left + 100.0f);
			bool flash = int(slope) % 2;

			spriteCmponent.effects[SpriteEffectId::RED] = {flash, 0};

		}
	}
};
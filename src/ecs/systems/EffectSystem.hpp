
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class EffectSystem : public System {
public:
	void update(uint ticks) {
		for (Entity entity : entities) {
			EffectComponent& effectComponent = ecs->getComponent<EffectComponent>(entity);

			for (int effect = 1; effect < Effect::count; effect++) {
				if (effectComponent.effects[effect] < ticks) effectComponent.effects[effect] = 0;
			}

			if (ecs->hasComponent<MovementComponent>(entity)) {
				MovementComponent& movementComponent = ecs->getComponent<MovementComponent>(entity);
				if (effectComponent.effects[Effect::SPEED]) {
					movementComponent.walkingSpeed = 4;
				} else {
					movementComponent.walkingSpeed = 2;
				}
			}
		}
	}
};
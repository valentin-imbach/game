
#pragma once
#include "System.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class DamageAreaSystem : public System {
public:
	void update() {
		for (Entity entity : entities) {
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			DamageAreaComponent& damageAreaComponent = ecs->getComponent<DamageAreaComponent>(entity);

			
		}
	}
};
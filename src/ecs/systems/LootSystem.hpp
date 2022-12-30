
#pragma once
#include "EntityFactory.hpp"
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class LootSystem : public System {
public:
	void update() {
		uint seed = SDL_GetTicks();
		for (Entity entity : entities) {
			HealthComponent& healthComponent = ecs->getComponent<HealthComponent>(entity);
			LootComponent& lootComponent = ecs->getComponent<LootComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			if (healthComponent.health <= 0) {
				for (int i = 0; i < lootComponent.lootTable.size; i++) {
					vec offset;
					offset.x = rand_float(seed++, -0.3f, 0.3f);
					offset.y = rand_float(seed++, -0.3f, 0.3f);
					lootComponent.lootTable.table[i].create(SDL_GetTicks(), positionComponent.position + offset);
				}
			}
		}
	}
};
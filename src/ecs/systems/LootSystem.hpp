
#pragma once
#include "EntityFactory.hpp"
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class LootSystem : public System {
public:
	void update(uint ticks, Realm* realm) {
		uint seed = ticks;
		for (Entity entity : entities) {
			LootComponent& lootComponent = ecs->getComponent<LootComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			for (int i = 0; i < lootComponent.lootTable.size; i++) {
				vec offset;
				offset.x = noise::Float(seed++, -0.3f, 0.3f);
				offset.y = noise::Float(seed++, -0.3f, 0.3f);
				lootComponent.lootTable.table[i].create(seed++, realm, positionComponent.position + offset);
			}
		}
	}
};
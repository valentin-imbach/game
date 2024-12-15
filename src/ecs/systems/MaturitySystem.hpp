
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class MaturitySystem : public System {

	ROSTER(MATURITY)

	void update(uint ticks, EntitySet& set) {
		for (Entity entity : set) {
			if (entities.find(entity) == entities.end()) continue;
			MaturityComponent& maturityComponent = ecs->getComponent<MaturityComponent>(entity);
			uint past = ticks - maturityComponent.birth;
			maturityComponent.maturity = std::min(maturityComponent.maxMaturity, ushort(past / maturityComponent.delay));
		}
	}

	// void changeSprite(Entity entity, ushort maturity) {
	// 	if (!ecs->hasComponent<SpriteComponent>(entity)) return;
	// 	SpriteComponent& spriteComponent = ecs->getComponent<SpriteComponent>(entity);
	// 	for (auto& layer : spriteComponent.spriteStack.stack) {
	// 		layer.sprite.source.x += layer.sprite.size.x * layer.sprite.frameCount;
	// 	}
		
	// }
};
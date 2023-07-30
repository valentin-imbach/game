
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class MaturitySystem : public System {
public:
	void update(uint ticks, EntitySet& set) {
		for (Entity entity : set) {
			if (entities.find(entity) == entities.end()) continue;
			MaturityComponent& maturityComponent = ecs->getComponent<MaturityComponent>(entity);
			if (maturityComponent.maturity == maturityComponent.maxMaturity) continue;
			if (ticks > maturityComponent.lastChange + maturityComponent.maturityTime) {
				maturityComponent.maturity += 1;
				maturityComponent.lastChange = ticks;
				changeSprite(entity, maturityComponent.maturity);
			}
		}
	}

	void changeSprite(Entity entity, ushort maturity) {
		if (!ecs->hasComponent<SpriteComponent>(entity)) return;
		SpriteComponent& spriteComponent = ecs->getComponent<SpriteComponent>(entity);
		for (auto& layer : spriteComponent.spriteStack.stack) {
			layer.first.source.x += layer.first.size.x * layer.first.frameCount;
		}
		
	}
};
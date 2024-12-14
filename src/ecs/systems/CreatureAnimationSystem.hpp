
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class CreatureAnimationSystem : public System {
public:
	void update(uint ticks) {
		for (Entity entity : entities) {
			MovementComponent& movementComponent = ecs->getComponent<MovementComponent>(entity);
			FacingComponent& facingComponent = ecs->getComponent<FacingComponent>(entity);
			CreatureAnimationComponent& creatureAnimationComponent = ecs -> getComponent<CreatureAnimationComponent>(entity);
			SpriteComponent& spriteComponent = ecs -> getComponent<SpriteComponent>(entity);

			auto spritePair = creatureAnimationComponent.sprites[movementComponent.movementState];
			SpriteStack spriteStack = (facingComponent.facing.x > 0) ? spritePair.first : spritePair.second;
			for (int i = 0; i < SPRITE_LAYER_COUNT; i++) {
				spriteStack.stack[i].sprite.animationStart = movementComponent.movementStart;
			}
			spriteComponent.spriteStack = spriteStack;
		}
	}
};
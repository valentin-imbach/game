
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class CreatureAnimationSystem : public System {

	ROSTER(MOVEMENT, FACING, CREATURE_ANIMATION, SPRITE)

	void update(uint ticks) {
		for (Entity entity : entities) {
			MovementComponent& movementComponent = ecs->getComponent<MovementComponent>(entity);
			FacingComponent& facingComponent = ecs->getComponent<FacingComponent>(entity);
			CreatureAnimationComponent& creatureAnimationComponent = ecs -> getComponent<CreatureAnimationComponent>(entity);
			SpriteComponent& spriteComponent = ecs -> getComponent<SpriteComponent>(entity);

			auto spritePair = creatureAnimationComponent.movements[movementComponent.movementState];
			for (int i = 0; i < SPRITE_LAYER_COUNT; i++) {
				spritePair.stack[i].sprite.animationStart = movementComponent.movementStart;
			}
			
			spriteComponent.spriteStack = spritePair;
			spriteComponent.flip = (facingComponent.facing.x <= 0);
		}
	}
};
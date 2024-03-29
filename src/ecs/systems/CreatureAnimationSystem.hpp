
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class CreatureAnimationSystem : public System {
public:
	void update(uint ticks) {
		for (Entity entity : entities) {
			MovementComponent& movementComponent = ecs->getComponent<MovementComponent>(entity);
			CreatureAnimationComponent& creatureAnimationComponent = ecs -> getComponent<CreatureAnimationComponent>(entity);
			SpriteComponent& spriteComponent = ecs -> getComponent<SpriteComponent>(entity);

			auto spritePair = creatureAnimationComponent.sprites[movementComponent.movementState];
			SpriteStack spriteStack = (movementComponent.facing == Direction::EAST) ? spritePair.first : spritePair.second;
			for (int i = 0; i < SPRITE_LAYER_COUNT; i++) {
				spriteStack.stack[i].sprite.animationStart = movementComponent.movementStart;
			}
			spriteComponent.spriteStack = spriteStack;

			// for (auto& layer : spriteComponent.spriteStack.stack) {
			// 	Sprite& sprite = layer.sprite;
			// 	if (movementComponent.movementState == MovementState::IDLE) {
			// 		sprite.frameCount = 1;
			// 	} else if (movementComponent.movementState == MovementState::WALK || movementComponent.movementState == MovementState::RUN) {
			// 		sprite.frameCount = 8;
			// 	}

			// 	sprite.source.y = (movementComponent.facing == Direction::EAST ? 0 : 2);
			// 	if (movementComponent.movementStart == ticks) sprite.animationReset(ticks - sprite.frameDuration);
			// }
		}
	}
};
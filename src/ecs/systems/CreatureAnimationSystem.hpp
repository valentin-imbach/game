
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class CreatureAnimationSystem : public System {
public:
	void update(uint ticks) {
		for (Entity entity : entities) {
			CreatureStateComponent& creatureStateComponent = ecs -> getComponent<CreatureStateComponent>(entity);
			CreatureAnimationComponent& creatureAnimationComponent = ecs -> getComponent<CreatureAnimationComponent>(entity);
			SpriteComponent& spriteComponent = ecs -> getComponent<SpriteComponent>(entity);

			auto spritePair = creatureAnimationComponent.sprites[creatureStateComponent.movementState];
			Sprite sprite = (creatureStateComponent.facing == Direction::EAST) ? spritePair.first : spritePair.second;
			sprite.animationStart = creatureStateComponent.lastChange;
			spriteComponent.spriteStack.setSprite(0, sprite, {0, -1});

			// for (auto& layer : spriteComponent.spriteStack.stack) {
			// 	Sprite& sprite = layer.sprite;
			// 	if (creatureStateComponent.movementState == MovementState::IDLE) {
			// 		sprite.frameCount = 1;
			// 	} else if (creatureStateComponent.movementState == MovementState::WALK || creatureStateComponent.movementState == MovementState::RUN) {
			// 		sprite.frameCount = 8;
			// 	}

			// 	sprite.source.y = (creatureStateComponent.facing == Direction::EAST ? 0 : 2);
			// 	if (creatureStateComponent.lastChange == ticks) sprite.animationReset(ticks - sprite.frameDuration);
			// }
		}
	}
};
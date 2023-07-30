
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class CreatureAnimationSystem : public System {
public:
	void update(uint ticks) {
		for (Entity entity : entities) {
			CreatureStateComponent& creatureStateComponent = ecs -> getComponent<CreatureStateComponent>(entity);
			SpriteComponent& spriteComponent = ecs -> getComponent<SpriteComponent>(entity);

			for (auto& layer : spriteComponent.spriteStack.stack) {
				Sprite& sprite = layer.first;
				if (creatureStateComponent.state == CreatureState::IDLE) {
					sprite.frameCount = 1;
				} else if (creatureStateComponent.state == CreatureState::WALKING || creatureStateComponent.state == CreatureState::RUNNING) {
					sprite.frameCount = 8;
				}

				sprite.source.y = (creatureStateComponent.facing == Direction::EAST ? 0 : 2);
				if (creatureStateComponent.lastChange == ticks) sprite.animationReset(ticks - sprite.frameDuration);
			}
		}
	}
};
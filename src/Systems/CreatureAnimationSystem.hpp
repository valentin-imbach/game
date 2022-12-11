
#pragma once
#include "../System.hpp"
#include "../ECS.hpp"

class CreatureAnimationSystem : public System {
public:
	void update() {
		for (Entity entity : entities) {
			SpriteStack& spriteStack = ecs -> getComponent<SpriteComponent>(entity).spriteStack;
			CreatureState state = ecs -> getComponent<CreatureStateComponent>(entity).state;
			bool stateChanged = ecs -> getComponent<CreatureStateComponent>(entity).stateChanged;
			Direction facing = ecs -> getComponent<CreatureStateComponent>(entity).facing;

			for (auto& layer : spriteStack.stack) {
				Sprite& sprite = layer.first;
				if (state == CreatureState::IDLE) {
					sprite.frameCount = 1;
					sprite.source.x = 7;
				} else if (state == CreatureState::WALKING) {
					sprite.frameCount = 8;
					sprite.source.x = 0;
				}

				sprite.source.y = facing == Direction::EAST ? 0 : 2;

				if (stateChanged) {
					sprite.animationReset();
				}
			}
		}
	}
};
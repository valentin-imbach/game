
#pragma once
#include "../System.hpp"

class CreatureAnimationSystem : public System {
public:
	void update() {
		for (Entity entity : entities) {
			SpriteStack& spriteStack = componentManager->get<SpriteComponent>(entity).spriteStack;
			CreatureState state = componentManager->get<CreatureStateComponent>(entity).state;
			bool stateChanged = componentManager->get<CreatureStateComponent>(entity).stateChanged;
			Direction facing = componentManager->get<CreatureStateComponent>(entity).facing;

			for (auto& layer : spriteStack.stack) {
				Sprite& sprite = layer.first;
				if (state == CreatureState::IDLE) {
					sprite.frameCount = 1;
					sprite.source.x = 7;
				} else if (state == CreatureState::WALKING) {
					sprite.frameCount = 8;
					sprite.source.x = 0;
				}

				sprite.source.y = int(facing) / 2;

				if (stateChanged) {
					sprite.animationReset();
				}
			}
		}
	}
};
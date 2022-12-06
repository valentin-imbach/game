
#pragma once
#include "../System.hpp"
#include "../Components.hpp"

class CreatureAnimationSystem : public System {
public:
	void update() {
		for (Entity entity : entities) {
			Sprite& sprite = componentManager->get<SpriteComponent>(entity).sprite;
			CreatureState state = componentManager->get<CreatureStateComponent>(entity).state;
			bool stateChanged = componentManager->get<CreatureStateComponent>(entity).stateChanged;
			Direction facing = componentManager->get<CreatureStateComponent>(entity).facing;

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
};

#pragma once
#include "../System.hpp"
#include "../Components.hpp"
#include "../Window.hpp"

class ControllerSystem : public System {
public:
	void update() {
		for (Entity entity : entities) {
			CreatureState& state = componentManager->get<CreatureStateComponent>(entity).state;
			Direction& direction = componentManager->get<DirectionComponent>(entity).direction;

			pair sum(1, 1);
			if (Window::instance->keyState[SDL_SCANCODE_W]) sum.y -= 1;
			if (Window::instance->keyState[SDL_SCANCODE_A]) sum.x -= 1;
			if (Window::instance->keyState[SDL_SCANCODE_S]) sum.y += 1;
			if (Window::instance->keyState[SDL_SCANCODE_D]) sum.x += 1;

			if (sum.x == 2 && sum.y == 1) direction = Direction(0);
			if (sum.x == 2 && sum.y == 0) direction = Direction(1);
			if (sum.x == 1 && sum.y == 0) direction = Direction(2);
			if (sum.x == 0 && sum.y == 0) direction = Direction(3);
			if (sum.x == 0 && sum.y == 1) direction = Direction(4);
			if (sum.x == 0 && sum.y == 2) direction = Direction(5);
			if (sum.x == 1 && sum.y == 2) direction = Direction(6);
			if (sum.x == 2 && sum.y == 2) direction = Direction(7);

			if (sum.x == 1 && sum.y == 1) {
				state = CreatureState::IDLE;
			} else {
				state = CreatureState::WALKING;
			}	
		}
	}
};
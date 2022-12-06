
#pragma once
#include "../System.hpp"
#include "../Components.hpp"

class CameraSystem : public System {
public:
	void update() {
		for (Entity entity : entities) {
			vec& position = componentManager->get<PositionComponent>(entity).position;
			Entity target = componentManager->get<CameraComponent>(entity).target;

			if (!componentManager->has<PositionComponent>(target)) return;
			vec targetPosition = componentManager->get<PositionComponent>(target).position;
			position = targetPosition;
		}
	}
};

#pragma once
#include "../System.hpp"
#include "../ECS.hpp"

class CameraSystem : public System {
public:
	void update() {
		for (Entity entity : entities) {
			vec& position = ecs -> getComponent<PositionComponent>(entity).position;
			Entity target = ecs -> getComponent<CameraComponent>(entity).target;

			if (!ecs -> hasComponent<PositionComponent>(target)) return;
			vec targetPosition = ecs -> getComponent<PositionComponent>(target).position;
			position = targetPosition;
		}
	}
};
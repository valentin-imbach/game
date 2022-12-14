
#pragma once
#include "ECS_types.hpp"
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class CameraSystem : public System {
public:
	void update(Entity player) {
		if (!player) return;
		vec targetPosition = ecs -> getComponent<PositionComponent>(player).position;
		for (Entity entity : entities) {
			vec& position = ecs -> getComponent<PositionComponent>(entity).position;
			position = targetPosition;
		}
	}

	Entity getCamera() {
		if (entities.empty()) return 0;
		return *entities.begin();
	}
};
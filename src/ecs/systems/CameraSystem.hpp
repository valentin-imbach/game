
#pragma once
#include "ECS_types.hpp"
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class CameraSystem : public System {
public:
	void update(Entity target) {
		if (!target) return;
		vec targetPosition = ecs -> getComponent<PositionComponent>(target).position;
		for (Entity entity : entities) {
			PositionComponent& positionComponent = ecs -> getComponent<PositionComponent>(entity);
			positionComponent.position = targetPosition;
		}
	}

	Entity getCamera() {
		if (entities.empty()) return 0;
		return *entities.begin();
	}
};

#pragma once
#include "CollisionSystem.hpp"
#include "System.hpp"
#include "Components.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"
#include "ECS.hpp"

class ColliderDrawSystem : public System {
public:
	void update(Entity camera) {
		if (!camera) return;
		vec cameraPosition = ecs -> getComponent<PositionComponent>(camera).position;
		float zoom = ecs -> getComponent<CameraComponent>(camera).zoom;
		pair screenSize = Window::instance->size;
		int border = 5 * BIT * zoom;

		for (Entity entity : entities) {
			Collider collider = ecs -> getComponent<ColliderComponent>(entity).collider;
			vec position = ecs -> getComponent<PositionComponent>(entity).position + collider.offset;

			pair screenPosition = round(BIT * zoom * (position  - cameraPosition)) + (Window::instance->size) / 2;
			pair size = round(BIT * zoom * collider.size);

			if (screenPosition.x + border < 0 || screenPosition.y + border < 0) continue;
			if (screenPosition.x > screenSize.x + border || screenPosition.y > screenSize.y + border) continue;

			TextureManager::drawRect(screenPosition, size, {255, 0, 0, 255});
		}
	}
};
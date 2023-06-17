
#pragma once
#include "CollisionSystem.hpp"
#include "System.hpp"
#include "Components.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"
#include "ECS.hpp"
#include "Camera.hpp"

class ColliderDrawSystem : public System {
public:
	void update(Camera camera, uint ticks) {
		pair screenSize = Window::instance->size;
		int border = 5 * BIT * camera.zoom;

		for (Entity entity : entities) {
			ColliderComponent& colliderComponent = ecs -> getComponent<ColliderComponent>(entity);
			PositionComponent& positionComponent = ecs -> getComponent<PositionComponent>(entity);
			pair screenPosition = camera.screenPosition( positionComponent.position + colliderComponent.collider.offset);

			if (screenPosition.x + border < 0 || screenPosition.y + border < 0) continue;
			if (screenPosition.x > screenSize.x + border || screenPosition.y > screenSize.y + border) continue;

			if (colliderComponent.collider.type == Shape::RECTANGLE) {
				pair size = vec::round(BIT * camera.zoom * colliderComponent.collider.size);
				TextureManager::drawRect(screenPosition, size, {255, 0, 0, 255});
			} else if (colliderComponent.collider.type == Shape::CIRCLE) {
				int radius = BIT * camera.zoom * colliderComponent.collider.radius;
				TextureManager::drawCirc(screenPosition, radius);
			}
		}
	}
};
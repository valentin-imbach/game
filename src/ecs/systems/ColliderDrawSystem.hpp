
#pragma once
#include "CollisionSystem.hpp"
#include "System.hpp"
#include "Components.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"
#include "ECS.hpp"

class ColliderDrawSystem : public System {
public:
	void update(Entity camera, uint ticks) {
		if (!camera) return;
		vec cameraPosition = ecs -> getComponent<PositionComponent>(camera).position;
		float cameraZoom = ecs -> getComponent<CameraComponent>(camera).zoom;
		pair screenSize = Window::instance->size;
		int border = 5 * BIT * cameraZoom;

		for (Entity entity : entities) {
			ColliderComponent& colliderComponent = ecs -> getComponent<ColliderComponent>(entity);
			PositionComponent& positionComponent = ecs -> getComponent<PositionComponent>(entity);

			vec position = positionComponent.position + colliderComponent.collider.offset;
			pair screenPosition = round(BIT * cameraZoom * (position  - cameraPosition)) + (Window::instance->size) / 2;
			

			if (screenPosition.x + border < 0 || screenPosition.y + border < 0) continue;
			if (screenPosition.x > screenSize.x + border || screenPosition.y > screenSize.y + border) continue;

			if (colliderComponent.collider.type == Shape::RECTANGLE) {
				pair size = round(BIT * cameraZoom * colliderComponent.collider.size);
				TextureManager::drawRect(screenPosition, size, {255, 0, 0, 255});
			} else if (colliderComponent.collider.type == Shape::CIRCLE) {
				int radius = BIT * cameraZoom * colliderComponent.collider.radius;
				TextureManager::drawCirc(screenPosition, radius);
			}
		}
	}
};
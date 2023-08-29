
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"
#include "ECS.hpp"
#include "Camera.hpp"

class DamageAreaDrawSystem : public System {
public:
	void update(Camera camera, uint ticks, EntitySet& set) {
		pair screenSize = Window::instance->size;
		int border = 5 * BIT * camera.zoom;

		for (Entity entity : set) {
			if (entities.find(entity) == entities.end()) continue;
			DamageAreaComponent& damageAreaComponent = ecs -> getComponent<DamageAreaComponent>(entity);
			PositionComponent& positionComponent = ecs -> getComponent<PositionComponent>(entity);
			pair screenPosition = camera.screenPosition(positionComponent.position + damageAreaComponent.shape.offset);

			if (screenPosition.x + border < 0 || screenPosition.y + border < 0) continue;
			if (screenPosition.x > screenSize.x + border || screenPosition.y > screenSize.y + border) continue;

			if (damageAreaComponent.shape.type == ShapeId::RECTANGLE) {
				pair size = vec::round(BIT * camera.zoom * damageAreaComponent.shape.size);
				TextureManager::drawRect(screenPosition, size, {255, 0, 0, 255});
			} else if (damageAreaComponent.shape.type == ShapeId::CIRCLE) {
				int radius = BIT * camera.zoom * damageAreaComponent.shape.radius;
				TextureManager::drawCirc(screenPosition, radius, {255, 0, 0, 255});
			}
		}
	}
};

#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"
#include "ECS.hpp"
#include "Camera.hpp"

class HitboxDrawSystem : public System {
public:
	void update(Camera camera, uint ticks, EntitySet& set) {
		pair screenSize = Window::instance->size;
		int border = 5 * BIT * camera.zoom;

		for (Entity entity : set) {
			if (entities.find(entity) == entities.end()) continue;
			HitboxComponent& hitboxComponent = ecs -> getComponent<HitboxComponent>(entity);
			PositionComponent& positionComponent = ecs -> getComponent<PositionComponent>(entity);
			vec screenPosition = camera.screenPosition(positionComponent.position + hitboxComponent.hitbox.offset);

			if (screenPosition.x + border < 0 || screenPosition.y + border < 0) continue;
			if (screenPosition.x > screenSize.x + border || screenPosition.y > screenSize.y + border) continue;

			if (hitboxComponent.hitbox.type == ShapeId::RECTANGLE) {
				pair size = vec::round(BIT * camera.zoom * hitboxComponent.hitbox.size);
				TextureManager::drawRect(screenPosition, size, {0, 255, 0, 255});
			} else if (hitboxComponent.hitbox.type == ShapeId::CIRCLE) {
				int radius = BIT * camera.zoom * hitboxComponent.hitbox.radius;
				TextureManager::drawCirc(screenPosition, radius, {0, 255, 0, 255});
			}
		}
	}
};
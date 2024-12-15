
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"
#include "ECS.hpp"
#include "Camera.hpp"

class ColliderDrawSystem : public System {

	ROSTER(COLLIDER, POSITION)

	void update(Camera camera, uint ticks, EntitySet& set) {
		pair screenSize = Window::instance->size;
		int border = 5 * BIT * camera.zoom;

		for (Entity entity : set) {
			if (entities.find(entity) == entities.end()) continue;
			ColliderComponent& colliderComponent = ecs -> getComponent<ColliderComponent>(entity);
			PositionComponent& positionComponent = ecs -> getComponent<PositionComponent>(entity);
			vec screenPosition = camera.screenPosition(positionComponent.position + colliderComponent.collider.offset);

			if (screenPosition.x + border < 0 || screenPosition.y + border < 0) continue;
			if (screenPosition.x > screenSize.x + border || screenPosition.y > screenSize.y + border) continue;

			if (colliderComponent.collider.type == ShapeId::RECTANGLE) {
				pair size = vec::round(BIT * camera.zoom * colliderComponent.collider.size);
				TextureManager::drawRect(screenPosition, size, {255, 255, 255, 255});
			} else if (colliderComponent.collider.type == ShapeId::CIRCLE) {
				int radius = BIT * camera.zoom * colliderComponent.collider.radius;
				TextureManager::drawCirc(screenPosition, radius, {255, 255, 255, 255});
			}

			// if (ecs->hasComponent<AiMoveComponent>(entity)) {
			// 	AiMoveComponent& aiMoveComponent = ecs->getComponent<AiMoveComponent>(entity);
			// 	pair start = camera.screenPosition(positionComponent.position);
			// 	for (int dir = 1; dir <= 8; dir++) {
			// 		float score = aiMoveComponent.moveScores[dir];
			// 		pair end = camera.screenPosition(positionComponent.position + std::abs(score) * Direction::unit[dir]);
			// 		if (score >= 0) {
			// 			SDL_SetRenderDrawColor(Window::instance->renderer, 0, 0, 255, 255);
			// 		} else {
			// 			SDL_SetRenderDrawColor(Window::instance->renderer, 255, 0, 0, 255);
			// 		}
			// 		SDL_RenderDrawLine(Window::instance->renderer, start.x, start.y, end.x, end.y);
			// 	}
				
			// }
		}
	}
};
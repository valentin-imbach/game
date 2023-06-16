
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "Item.hpp"
#include "Window.hpp"

class HandRenderSystem : public System {
public:
	void update(Camera camera, std::vector<DrawCall>& drawQueue, uint ticks) {
		// vec cameraPosition = ecs->getComponent<PositionComponent>(camera).position;
		// float zoom = ecs->getComponent<CameraComponent>(camera).zoom;
		pair screenSize = Window::instance->size;

		for (Entity entity : entities) {
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			PlayerComponent& playerComponent = ecs->getComponent<PlayerComponent>(entity);
			CreatureStateComponent& creatureStateComponent = ecs->getComponent<CreatureStateComponent>(entity);

			Entity item = playerComponent.hotbar.itemContainers[playerComponent.activeSlot][0].item;
			if (!item) continue;
			SpriteStack& itemSprites = ecs->getComponent<SpriteComponent>(item).spriteStack;

			vec entityPosition = positionComponent.position;
			TextureStyle style;

			entityPosition.y -= 0.5;
			if (creatureStateComponent.facing == Direction::EAST) {
				entityPosition.x += 0.2;
			} else if (creatureStateComponent.facing == Direction::WEST) {
				entityPosition.x -= 0.2;
				style.flip = SDL_FLIP_HORIZONTAL;
			}

			// if (creatureStateComponent.state == CreatureState::WALKING) {
			// 	uint past = ticks - creatureStateComponent.lastChange;
			// 	entityPosition.y += 0.05 * sin(float(past) / 100);
			// }

			pair screenPosition = camera.screenPosition(entityPosition);
			drawQueue.push_back({itemSprites, screenPosition, 0, int(camera.zoom), style});
		}
	}
};

#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "Item.hpp"
#include "Window.hpp"

class HandRenderSystem : public System {
public:
	void update(Camera camera, std::vector<DrawCall>& drawQueue, uint ticks) {
		for (Entity entity : entities) {
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			PlayerComponent& playerComponent = ecs->getComponent<PlayerComponent>(entity);
			CreatureStateComponent& creatureStateComponent = ecs->getComponent<CreatureStateComponent>(entity);

			Entity item = playerComponent.hotbar.itemContainers[playerComponent.activeSlot][0].item;

			if (!item) continue;
			ItemComponent& itemComponent = ecs->getComponent<ItemComponent>(item);
			if (!itemComponent.show) continue;

			SpriteStack& itemSprites = ecs->getComponent<SpriteComponent>(item).spriteStack;

			vec entityPosition = positionComponent.position;
			TextureStyle style;

			entityPosition.y -= 0.5;

			if (creatureStateComponent.actionState != ActionState::ATTACK) continue;

			uint timePassed = ticks - creatureStateComponent.actionStart;

			if (creatureStateComponent.facing == Direction::EAST) {
				entityPosition.x += 0.625;
				style.pivot = pair(-8 * camera.zoom, 8 * camera.zoom);
				style.angle = -90;
				if (timePassed < 300) style.angle = -90 + std::sin((M_PI * timePassed) / 300) * 90;
			} else if (creatureStateComponent.facing == Direction::WEST) {
				entityPosition.x -= 0.625;
				style.flip = SDL_FLIP_HORIZONTAL;
				style.pivot = pair(8 * camera.zoom, 8 * camera.zoom);
				style.angle = 90;
				if (timePassed < 300) style.angle = 90 - std::sin((M_PI * timePassed) / 300) * 90;
			}
			
			// if (creatureStateComponent.movementState == MovementState::WALK) {
			// 	uint past = ticks - creatureStateComponent.movementStart;
			// 	entityPosition.y += 0.05 * sin(float(past) / 100);
			// }

			pair screenPosition = camera.screenPosition(entityPosition);
			drawQueue.push_back({itemSprites, screenPosition, 1, int(camera.zoom), style});
		}
	}
};
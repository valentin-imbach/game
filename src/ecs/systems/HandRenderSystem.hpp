
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
			ActionComponent& actionComponent = ecs->getComponent<ActionComponent>(entity);
			HandComponent& handComponent = ecs->getComponent<HandComponent>(entity);
			MovementComponent& movementComponent = ecs->getComponent<MovementComponent>(entity);

			Entity item = handComponent.item;
			if (actionComponent.actionState != ActionState::IDLE) {
				item = actionComponent.item;
			}

			if (!item) continue;
			ItemComponent& itemComponent = ecs->getComponent<ItemComponent>(item);
			if (!itemComponent.show) continue;

			TextureStyle style;

			pair mpos = Window::instance->mousePosition;
			pair epos = camera.screenPosition(positionComponent.position);
			style.angle = vec::angle(mpos - epos);

			vec pos = positionComponent.position;
			pos.y -= 0.5;

			int pm = 1;
			if (movementComponent.facing == Direction::WEST) {
				pm = -1;
				style.flip = SDL_FLIP_HORIZONTAL;
				style.angle += M_PI;
			}

			pos.x += pm * 0.5;
			style.pivot = pair(-pm * 8 * camera.zoom, 0);
			
			if (actionComponent.actionState == ActionState::ATTACK) {
				uint timePassed = ticks - actionComponent.start;
				uint totalTime = actionComponent.end - actionComponent.start;
				float t = float(timePassed)/totalTime;

				float angle = std::sin(2 * t*t * M_PI) * M_PI * (1-t);
				style.angle += pm * angle;
			}
			
			// if (movementComponent.movementState == MovementState::WALK) {
			// 	uint past = ticks - movementComponent.movementStart;
			// 	entityPosition.y += 0.05 * sin(float(past) / 100);
			// }

			pair screenPosition = camera.screenPosition(pos);
			SpriteStack& itemSprites = ecs->getComponent<SpriteComponent>(item).spriteStack;
			drawQueue.push_back({itemSprites, screenPosition, 1, int(camera.zoom), style});
		}
	}
};
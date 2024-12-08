
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
			style.angle = 0;

			if (ecs->hasComponent<PlayerComponent>(entity)) {
				vec mpos = Window::instance->mousePosition;
				vec epos = camera.screenPosition(positionComponent.position);
				if (mpos.x > epos.x) {
					style.angle = vec::angle(mpos - epos);
				} else {
					style.angle = M_PI - vec::angle(mpos - epos);
				}
			}

			vec offset = vec(0.5f, -0.5f);
			style.pivot = pair(-8 * camera.zoom, 0);
					
			if (actionComponent.actionState == ActionState::ATTACK) {
				uint timePassed = ticks - actionComponent.start;
				uint totalTime = actionComponent.end - actionComponent.start;
				float t = float(timePassed)/totalTime;
				style.angle += std::sin(2 * t*t * M_PI) * M_PI * (1-t);
			} else if (actionComponent.actionState == ActionState::WIND_UP) {
				offset.x = 0.2f;
				style.pivot = pair(0, 0);
				style.angle += M_PI_4;
			}

			if (movementComponent.facing == Direction::WEST) {
				offset.x *= -1;
				style.angle *= -1;
				style.pivot.x *= -1;
				style.flip = SDL_FLIP_HORIZONTAL;
			}
			
			// if (movementComponent.movementState == MovementState::WALK) {
			// 	uint past = ticks - movementComponent.movementStart;
			// 	entityPosition.y += 0.05 * sin(float(past) / 100);
			// }

			vec screenPosition = camera.screenPosition(positionComponent.position + offset);
			SpriteStack& itemSprites = ecs->getComponent<SpriteComponent>(item).spriteStack;
			drawQueue.push_back({itemSprites, screenPosition, 1, int(camera.zoom), style});
		}
	}
};

#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"
#include "ECS.hpp"
#include "Camera.hpp"
#include "Sprite.hpp"

#define PIXEL_PERFECT false

class EntityDrawSystem : public System {

	ROSTER(POSITION, SPRITE);

	void update(Camera camera, std::vector<DrawCall>& drawQueue, bool active, Entity player, uint ticks, EntitySet& set) {
		pair screenSize = Window::instance->size;
		int border = 5 * BIT * camera.zoom;

		for (Entity entity : set) {
			if (entities.find(entity) == entities.end()) continue;
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			SpriteComponent& spriteComponent = ecs->getComponent<SpriteComponent>(entity);

			vec entityPosition = positionComponent.position;

			// vec roughScreenPosition = camera.screenPosition(entityPosition);
			// if (roughScreenPosition.x + border < 0 || roughScreenPosition.y + border < 0) continue;
			// if (roughScreenPosition.x > screenSize.x + border || roughScreenPosition.y > screenSize.y + border) continue;

		
			TextureStyle style;
			style.centered = false;
			style.angle = spriteComponent.angle;

			if (spriteComponent.flip) style.flip = SDL_FLIP_HORIZONTAL;
			
			if (spriteComponent.effects[SpriteEffectId::SHAKE].first) {
				uint past = ticks - spriteComponent.effects[SpriteEffectId::SHAKE].second;
				if (past > 500) {
					spriteComponent.effects[SpriteEffectId::SHAKE] = {false, 0};
				} else {
					entityPosition += vec(3 * sinf(float(past) / 30) / (past + 1), 0);
				}
			}
			
			if (spriteComponent.effects[SpriteEffectId::BOUNCE].first) entityPosition += vec(0, sinf(float(ticks) / 200) / 30);
			if (spriteComponent.effects[SpriteEffectId::RED].first) style.tint = {255, 100, 100};
			if (spriteComponent.effects[SpriteEffectId::HIGHLIGHT].first) style.tint = {100, 100, 255};
			if (spriteComponent.effects[SpriteEffectId::HURT].first) style.tint = {255, 50, 50};
			if (spriteComponent.effects[SpriteEffectId::OUTLINE].first) style.outline = true;

			// if (active && ecs->hasComponent<GridComponent>(entity)) {
			// 	GridComponent& gridComponent = ecs->getComponent<GridComponent>(entity);
			// 	vec mpos = camera.worldPosition(Window::instance->mousePosition);
			// 	if (vec::inside(mpos, vec(gridComponent.anker) - vec(0.5f, 0.5f), gridComponent.size, false)) {
			// 		style.outline = true;
			// 	}
			// }

			auto bounds = spriteComponent.spriteStack.bounds();
			vec size = vec(bounds.second - bounds.first) - vec(1, 1.5f);
			if (active && size.y >= 1 && player) {
				vec playerPos = ecs->getComponent<PositionComponent>(player).position;
				float a1 = std::min(0.3f + 2 * vec::dist_to_rect(playerPos, entityPosition + bounds.first, size, false), 1.0f);
				vec mouseWorldPos = camera.worldPosition(Window::instance->mousePosition);
				size.y -= 0.5f;
				float a2 = std::min(0.3f + 2 * vec::dist_to_rect(mouseWorldPos, entityPosition + bounds.first, size, false), 1.0f);
				style.alpha = std::min(a1, a2);
			}

			// if (PIXEL_PERFECT) {
			// 	entityPosition = vec(round(BIT * entityPosition)) / BIT;
			// 	cameraPosition = vec(round(BIT * cameraPosition)) / BIT;
			// } else {
			// 	cameraPosition = vec(round(BIT * zoom * cameraPosition)) / (BIT * zoom);
			// }

			vec offset(0.5f, 0.5f);
			vec screenPosition = camera.screenPosition(entityPosition - spriteComponent.scale * offset);

			drawQueue.push_back({spriteComponent.spriteStack, screenPosition, BIT * camera.zoom * spriteComponent.z, int(spriteComponent.scale * camera.zoom), style, entity});
		}
	}
};
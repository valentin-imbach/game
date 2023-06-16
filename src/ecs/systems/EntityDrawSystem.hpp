
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"
#include "ECS.hpp"
#include "Camera.hpp"

#define PIXEL_PERFECT false

struct DrawCall {
	SpriteStack spriteStack;
	pair position;
	float z;
	int scale;
	TextureStyle style;
};

class EntityDrawSystem : public System {
public:
	void update(Camera camera, uint ticks, std::set<Entity>& chunk) {
		pair screenSize = Window::instance->size;
		int border = 5 * BIT * camera.zoom;

		std::vector<DrawCall> drawQueue;

		for (Entity entity : entities) {
			//if (chunk.find(entity) == chunk.end()) continue;
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			SpriteComponent& spriteComponent = ecs->getComponent<SpriteComponent>(entity);

			vec entityPosition = positionComponent.position;

			TextureStyle style;
			style.centered = false;
			style.angle = spriteComponent.angle;
			
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

			// if (PIXEL_PERFECT) {
			// 	entityPosition = vec(round(BIT * entityPosition)) / BIT;
			// 	cameraPosition = vec(round(BIT * cameraPosition)) / BIT;
			// } else {
			// 	cameraPosition = vec(round(BIT * zoom * cameraPosition)) / (BIT * zoom);
			// }

			vec offset(0.5f, 0.5f);
			pair screenPosition = camera.screenPosition(entityPosition - spriteComponent.scale * offset);
			
			if (screenPosition.x + border < 0 || screenPosition.y + border < 0) continue;
			if (screenPosition.x > screenSize.x + border || screenPosition.y > screenSize.y + border) continue;

			drawQueue.push_back({spriteComponent.spriteStack, screenPosition, BIT * camera.zoom * spriteComponent.z, int(spriteComponent.scale * camera.zoom), style});
		}

		auto lambda = [](auto& l, auto& r) {
			float ly = l.position.y + l.z;
			float ry = r.position.y + r.z;
			return ly < ry || (ly == ry && l.position.x < r.position.x);
		};

		std::sort(drawQueue.begin(), drawQueue.end(), lambda);
		for (auto& p : drawQueue) {
			p.spriteStack.draw(p.position, p.scale, p.style, ticks);
		}
	}
};
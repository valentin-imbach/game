
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"
#include "ECS.hpp"

struct DrawCall {
	SpriteStack spriteStack;
	pair position;
	float z;
	int scale;
	TextureStyle style;
};

class EntityDrawSystem : public System {
public:
	void update(Entity camera, uint ticks, std::set<Entity>& chunk) {
		if (!camera) return;
		vec cameraPosition = ecs->getComponent<PositionComponent>(camera).position;
		float zoom = ecs->getComponent<CameraComponent>(camera).zoom;
		pair screenSize = Window::instance->size;
		int border = 5 * BIT * zoom;

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

			vec offset(0.5f, 0.5f);
			pair screenPosition = round(BIT * zoom * (entityPosition - spriteComponent.scale * offset - cameraPosition)) + (Window::instance->size) / 2;

			if (screenPosition.x + border < 0 || screenPosition.y + border < 0) continue;
			if (screenPosition.x > screenSize.x + border || screenPosition.y > screenSize.y + border) continue;

			drawQueue.push_back({spriteComponent.spriteStack, screenPosition, BIT * zoom * spriteComponent.z, int(spriteComponent.scale * zoom), style});
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
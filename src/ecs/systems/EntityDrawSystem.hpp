
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"
#include "ECS.hpp"

struct DrawCall {
	SpriteStack spriteStack;
	pair position;
	int scale;
	bool centered;
};

class EntityDrawSystem : public System {
public:
	void update(Entity camera) {
		if (!camera) return;
		vec cameraPosition = ecs->getComponent<PositionComponent>(camera).position;
		float zoom = ecs->getComponent<CameraComponent>(camera).zoom;
		pair screenSize = Window::instance->size;
		int border = 5 * BIT * zoom;

		std::vector<std::pair<float, DrawCall>> drawQueue;

		for (Entity entity : entities) {
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			SpriteComponent& spriteComponent = ecs->getComponent<SpriteComponent>(entity);

			vec entityPosition = positionComponent.position;
			if (spriteComponent.shader.shaderId == ShaderId::SHAKE) {
				uint past = SDL_GetTicks() - spriteComponent.shader.start;
				if (past > 500) {
					spriteComponent.shader = {ShaderId::NONE, 0};
				} else {
					entityPosition += vec(3 * sinf(float(past) / 30) / (past + 1), 0);
				}
			} else if (spriteComponent.shader.shaderId == ShaderId::BOUNCE) {
				entityPosition += vec(0, sinf(float(SDL_GetTicks()) / 200) / 30);
			}

			vec offset = {0.5f, spriteComponent.height + 0.5f};
			pair screenPosition = round(BIT * zoom * (entityPosition - spriteComponent.scale * offset - cameraPosition)) + (Window::instance->size) / 2;

			if (screenPosition.x + border < 0 || screenPosition.y + border < 0) continue;
			if (screenPosition.x > screenSize.x + border || screenPosition.y > screenSize.y + border) continue;

			DrawCall drawCall = {spriteComponent.spriteStack, screenPosition, int(spriteComponent.scale * zoom), false};
			drawQueue.emplace_back(positionComponent.position.y, drawCall);
		}

		auto lambda = [](auto& l, auto& r) { return l.first < r.first; };
		std::sort(drawQueue.begin(), drawQueue.end(), lambda);
		for (auto& p : drawQueue) p.second.spriteStack.draw(p.second.position, p.second.scale, p.second.centered);
	}
};
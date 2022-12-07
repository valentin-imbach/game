
#pragma once
#include <cmath>
#include "../System.hpp"
#include "../ComponentManager.hpp"
#include "../Components.hpp"
#include "../TextureManager.hpp"
#include "../Window.hpp"

class SpriteSystem : public System {
public:
	void update(Entity camera, std::vector<std::pair<float, DrawCall>>& drawQueue) {
		vec cameraPosition = componentManager->get<PositionComponent>(camera).position;
		float zoom = componentManager->get<CameraComponent>(camera).zoom;
		pair screenSize = Window::instance->size;
		int border = 5 * BIT * zoom;

		for (Entity entity : entities) {
			vec entityPosition = componentManager->get<PositionComponent>(entity).position;
			SpriteStack spriteStack = componentManager->get<SpriteComponent>(entity).spriteStack;
			uint8_t height = componentManager->get<SpriteComponent>(entity).height;
			float scale = componentManager->get<SpriteComponent>(entity).scale;

			vec offset = {0.5f, height + 0.5f};
			pair screenPosition = round(BIT * zoom * (entityPosition - scale * offset - cameraPosition)) + (Window::instance->size) / 2;

			if (screenPosition.x + border < 0 || screenPosition.y + border < 0) continue;
			if (screenPosition.x > screenSize.x + border || screenPosition.y > screenSize.y + border) continue;

			DrawCall drawCall = {spriteStack, screenPosition, int(scale * zoom), false};
			drawQueue.emplace_back(entityPosition.y, drawCall);
		}
	}
};
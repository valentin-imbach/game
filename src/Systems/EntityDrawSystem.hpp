
#pragma once
#include "../System.hpp"
#include "../TextureManager.hpp"
#include "../Window.hpp"
#include "../ECS.hpp"

struct DrawCall {
	SpriteStack spriteStack;
	pair position;
	int scale;
	bool centered;
};

class EntityDrawSystem : public System {
public:
	void update(Entity camera) {
		vec cameraPosition = ecs -> getComponent<PositionComponent>(camera).position;
		float zoom = ecs -> getComponent<CameraComponent>(camera).zoom;
		pair screenSize = Window::instance->size;
		int border = 5 * BIT * zoom;

		std::vector<std::pair<float, DrawCall>> drawQueue;

		for (Entity entity : entities) {
			vec entityPosition = ecs -> getComponent<PositionComponent>(entity).position;
			SpriteStack spriteStack = ecs -> getComponent<SpriteComponent>(entity).spriteStack;
			uint8_t height = ecs -> getComponent<SpriteComponent>(entity).height;
			float scale = ecs -> getComponent<SpriteComponent>(entity).scale;

			vec offset = {0.5f, height + 0.5f};
			pair screenPosition = round(BIT * zoom * (entityPosition - scale * offset - cameraPosition)) + (Window::instance->size) / 2;

			if (screenPosition.x + border < 0 || screenPosition.y + border < 0) continue;
			if (screenPosition.x > screenSize.x + border || screenPosition.y > screenSize.y + border) continue;

			DrawCall drawCall = {spriteStack, screenPosition, int(scale * zoom), false};
			drawQueue.emplace_back(entityPosition.y, drawCall);
		}

		std::sort(drawQueue.begin(), drawQueue.end(), [](auto& l, auto& r) { return l.first < r.first; });
		for (auto& p : drawQueue) {
			p.second.spriteStack.draw(p.second.position, p.second.scale, p.second.centered);
		}
	}
};
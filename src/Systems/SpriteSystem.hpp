
#pragma once
#include "../System.hpp"
#include "../ComponentManager.hpp"
#include "../Components.hpp"
#include "../TextureManager.hpp"
#include "../Window.hpp"

class SpriteSystem : public System {
public:
	void update(Entity camera) {
		for (Entity entity : entities) {
			vec entityPosition = componentManager->get<PositionComponent>(entity).position;
			Sprite sprite = componentManager->get<SpriteComponent>(entity).sprite;

			vec cameraPosition = componentManager->get<PositionComponent>(camera).position;
			int zoom = componentManager->get<CameraComponent>(camera).zoom;

			pair screenPosition = round(BIT * zoom * (entityPosition - cameraPosition)) + (Window::instance->size) / 2;
			sprite.draw(screenPosition, zoom, true);
		}
	}
};
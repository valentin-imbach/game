
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
			SpriteSheet spriteSheet = componentManager->get<SpriteComponent>(entity).spriteSheet;

			vec cameraPosition = componentManager->get<PositionComponent>(camera).position;
			int zoom = componentManager->get<CameraComponent>(camera).zoom;

			pair screenPosition = round(BIT * zoom * (entityPosition - cameraPosition)) + (Window::instance->size) / 2;
			TextureManager::drawTexture(spriteSheet, {0, 0}, {BIT, BIT}, screenPosition, zoom, true);
		}
	}
};
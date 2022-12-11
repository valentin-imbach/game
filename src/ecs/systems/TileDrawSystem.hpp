
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "Window.hpp"
#include "Map.hpp"
#include "ECS.hpp"

class TileDrawSystem : public System {
public:
	void update(Map& map) {
		for (Entity camera : entities) {
			vec cameraPosition = ecs -> getComponent<PositionComponent>(camera).position;
			int zoom = ecs -> getComponent<CameraComponent>(camera).zoom;
			pair screenSize = Window::instance->size;
			int border = BIT * zoom / 2;

			pair range = ceil(vec(screenSize) / (2 * BIT * zoom));
			pair start = round(cameraPosition);

			int x1 = std::max(0, start.x - range.x);
			int x2 = std::min(MAP_WIDTH - 1, start.x + range.x);
			int y1 = std::max(0, start.y - range.y);
			int y2 = std::min(MAP_HEIGHT - 1, start.y + range.y);

			for (int x = x1; x <= x2; x++) {
				for (int y = y1; y <= y2; y++) {
					SpriteStack spriteStack = map.tiles[x][y]->spriteStack;
					vec tilePosition = {x, y};
					pair screenPosition = round(BIT * zoom * (tilePosition - cameraPosition)) + screenSize / 2;
					spriteStack.draw(screenPosition, zoom, true);
				}
			}
		}
	}
};
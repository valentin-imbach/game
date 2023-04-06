
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "Window.hpp"
#include "Map.hpp"
#include "ECS.hpp"

class TileDrawSystem : public System {
public:
	void update(Map* map, uint ticks) {
		for (Entity camera : entities) {
			vec cameraPosition = ecs -> getComponent<PositionComponent>(camera).position;
			int zoom = ecs -> getComponent<CameraComponent>(camera).zoom;
			pair screenSize = Window::instance->size;
			int border = BIT * zoom / 2;

			pair range = ceil(vec(screenSize) / (2 * BIT * zoom));
			pair start = round(cameraPosition);

			int x1 = std::max(0, start.x - range.x);
			int x2 = std::min(map->size.x - 1, start.x + range.x);
			int y1 = std::max(0, start.y - range.y);
			int y2 = std::min(map->size.y - 1, start.y + range.y);

			for (int x = x1; x <= x2; x++) {
				for (int y = y1; y <= y2; y++) {
					for (auto& layer : map->tiles[x][y]->sprites) {
						vec tilePosition(x, y);
						pair screenPosition = round(BIT * zoom * (tilePosition - cameraPosition)) + screenSize / 2;
						layer.second.draw(screenPosition, zoom);
					}
				}
			}
		}
	}
};
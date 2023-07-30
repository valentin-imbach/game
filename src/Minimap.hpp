
#pragma once
#include "TextureManager.hpp"
#include "TextManager.hpp"
#include "Window.hpp"
#include "Realm.hpp"
#include "Events.hpp"

class Minimap {
public:
	bool handleEvent(InputEvent event) {
		if (event.id == InputEventId::MAP) {
			active = !active;
			return true;
		}

		if (active) {
			if (event.id == InputEventId::ZOOM_IN) {
				zoom += 1;
				return true;
			} else if (event.id == InputEventId::ZOOM_OUT) {
				zoom = std::max(zoom - 1, 1);
				return true;
			}
		}
		return false;
	}

	void update(Realm* r) {
		realm = r;
	}

	void draw(vec playerPos) {
		if (!active || !realm || !realm->map) return;

		pair mapSize = realm->map->size;
		TextureManager::drawTexture(realm->minimap, nullptr, {0,0}, mapSize, Window::instance->size/2, zoom * mapSize);
		TextureManager::drawCirc(worldToMap(playerPos), 5);

		SDL_SetRenderDrawColor(Window::instance->renderer, 255, 0, 0, 100);

		pair chunkNumber = realm->map->size/CHUNK_SIZE;
		for (int x = 0; x < chunkNumber.x; x++) {
			int xx = x * CHUNK_SIZE + CHUNK_SIZE/2;
			pair start = worldToMap(vec(xx - 0.5f, -1));
			pair end = worldToMap(vec(xx - 0.5f, realm->map->size.y + 0.5f));
			SDL_RenderDrawLine(Window::instance->renderer, start.x, start.y, end.x, end.y);
		}

		for (int y = 0; y < chunkNumber.y; y++) {
			int yy = y * CHUNK_SIZE + CHUNK_SIZE/2;
			pair start = worldToMap(vec(-1, yy - 0.5f));
			pair end = worldToMap(vec(realm->map->size.y + 0.5f, yy - 0.5f));
			SDL_RenderDrawLine(Window::instance->renderer, start.x, start.y, end.x, end.y);
		}
		
	}

	pair worldToMap(vec position) {
		return Window::instance->size/2 - zoom * realm->map->size/2 + vec::round(zoom * (position + vec(0.5f, 0.5f)));
	}

private:
	Realm* realm;
	int zoom = 1;
	bool active = false;
};
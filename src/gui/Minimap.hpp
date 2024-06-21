
#pragma once
#include "TextureManager.hpp"
#include "TextManager.hpp"
#include "Window.hpp"
#include "Events.hpp"

#include "Chunk.hpp"

struct MapData {
	SDL_Texture* texture;
	pair offset;
	pair size;
};

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

	void update(MapData data) {
		mapData = data;
	}

	void draw(vec playerPos) {
		if (!active) return;

		TextureManager::drawTexture(mapData.texture, nullptr, {0,0}, mapData.size, Window::instance->size/2, zoom * mapData.size);
		TextureManager::drawCirc(worldToMap(playerPos), 5);

		SDL_SetRenderDrawColor(Window::instance->renderer, 255, 0, 0, 100);

		pair chunkNumber = mapData.size / CHUNK_SIZE;
		for (int x = 0; x <= chunkNumber.x; x++) {
			int xx = x * CHUNK_SIZE + mapData.offset.x;
			pair start = worldToMap(vec(xx - 0.5f, -1 + mapData.offset.y));
			pair end = worldToMap(vec(xx - 0.5f, mapData.size.y + 0.5f + mapData.offset.y));
			SDL_RenderDrawLine(Window::instance->renderer, start.x, start.y, end.x, end.y);
		}

		for (int y = 0; y <= chunkNumber.y; y++) {
			int yy = y * CHUNK_SIZE + mapData.offset.y;
			pair start = worldToMap(vec(-1 + mapData.offset.x, yy - 0.5f));
			pair end = worldToMap(vec(mapData.size.y + 0.5f + mapData.offset.x, yy - 0.5f));
			SDL_RenderDrawLine(Window::instance->renderer, start.x, start.y, end.x, end.y);
		}

		// pair tl = worldToMap(vec(-0.5f, -0.5f));
		// pair tr = worldToMap(vec(size.x - 0.5f, -0.5f));
		// pair bl = worldToMap(vec(-0.5f, size.y - 0.5f));
		// pair br = worldToMap(vec(size.x - 0.5f, size.y - 0.5f));
		// SDL_SetRenderDrawColor(Window::instance->renderer, 0, 255, 0, 100);
		// SDL_RenderDrawLine(Window::instance->renderer, tl.x, tl.y, tr.x, tr.y);
		// SDL_RenderDrawLine(Window::instance->renderer, bl.x, bl.y, br.x, br.y);
		// SDL_RenderDrawLine(Window::instance->renderer, tl.x, tl.y, bl.x, bl.y);
		// SDL_RenderDrawLine(Window::instance->renderer, tr.x, tr.y, br.x, br.y);
	}

	pair worldToMap(vec position) {
		return (Window::instance->size - zoom * mapData.size)/2 + vec::round(zoom * (position + vec(0.5f, 0.5f) - mapData.offset));
	}

private:
	MapData mapData;

	int zoom = 1;
	bool active = false;

	friend class Realm;
};
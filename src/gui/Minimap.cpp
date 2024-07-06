
#include "Minimap.hpp"
#include "Realm.hpp"

bool Minimap::handleEvent(InputEvent event) {
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

void Minimap::refresh(Realm* realm) {
	std::unordered_map<pair, Chunk>& chunks = realm->chunkManager.chunks;
	if (chunks.empty()) return;

	pair topLeft = chunks.begin()->second.position;
	pair bottomRight = topLeft;

	for (auto [p, c] : chunks) {
		topLeft.x = std::min(topLeft.x, p.x);
		topLeft.y = std::min(topLeft.y, p.y);

		bottomRight.x = std::max(bottomRight.x, p.x);
		bottomRight.y = std::max(bottomRight.y, p.y);
	}

	size = CHUNK_SIZE * (bottomRight - topLeft + pair(1, 1));
	offset = CHUNK_SIZE * topLeft;

	SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, size.x, size.y, 32, SDL_PIXELFORMAT_RGBA8888);
	Uint32 *pixels = (Uint32 *)surface->pixels;

	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			pair pos = pair(x, y) + offset;
			GroundId::value groundId = realm->chunkManager.getGround(pos);
			pixels[y * size.x + x] = GroundTemplate::templates[groundId].colour;
		}
	}
	
	texture = SDL_CreateTextureFromSurface(Window::instance->renderer, surface);
	SDL_FreeSurface(surface);
}

void Minimap::update() {
	
}

void Minimap::draw(Realm* realm, vec playerPos) {
	if (!active) return;

	TextureManager::drawTexture(texture, nullptr, {0,0}, size, Window::instance->size/2, zoom * size);
	TextureManager::drawCirc(worldToMap(playerPos), 5);

	SDL_SetRenderDrawColor(Window::instance->renderer, 255, 0, 0, 100);

	pair chunkNumber = size / CHUNK_SIZE;
	for (int x = 0; x <= chunkNumber.x; x++) {
		int xx = x * CHUNK_SIZE + offset.x;
		pair start = worldToMap(vec(xx - 0.5f, -1 + offset.y));
		pair end = worldToMap(vec(xx - 0.5f, size.y + 0.5f + offset.y));
		SDL_RenderDrawLine(Window::instance->renderer, start.x, start.y, end.x, end.y);
	}

	for (int y = 0; y <= chunkNumber.y; y++) {
		int yy = y * CHUNK_SIZE + offset.y;
		pair start = worldToMap(vec(-1 + offset.x, yy - 0.5f));
		pair end = worldToMap(vec(size.y + 0.5f + offset.x, yy - 0.5f));
		SDL_RenderDrawLine(Window::instance->renderer, start.x, start.y, end.x, end.y);
	}

	for (int x = 0; x < chunkNumber.x; x++) {
		for (int y = 0; y < chunkNumber.y; y++) {
			pair chunk(x, y);
			pair position = CHUNK_SIZE * chunk + offset;
		
			

		}
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

pair Minimap::worldToMap(vec position) {
	return (Window::instance->size - zoom * size)/2 + vec::round(zoom * (position + vec(0.5f, 0.5f) - offset));
}
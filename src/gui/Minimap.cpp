
#include "Minimap.hpp"
#include "Realm.hpp"

Minimap::Minimap() {
	widget = std::make_unique<Widget>(pair(0,-20), pair(0,0), Sprite(), Direction::SOUTH);
	widget->emplaceGuiElement<CheckboxGui>(pair(-10,0), &chunkOverlay);
	widget->emplaceGuiElement<CheckboxGui>(pair(10,0), &tempOverlay);
}

bool Minimap::handleEvent(InputEvent event) {
	if (event.id == InputEventId::MAP) {
		active = !active;
		return true;
	}

	if (!active) return false;

	if (widget->handleEvent(event)) {
		return true;
	} else if (event.id == InputEventId::ZOOM_IN) {
		zoom += 1;
		return true;
	} else if (event.id == InputEventId::ZOOM_OUT) {
		zoom = std::max(zoom - 1, 1);
		return true;
	}

	return false;
}

void Minimap::update(vec playerPos, Realm* realm) {
	playerPosition = playerPos;
	playerRealm = realm;
	widget->reposition();
	widget->update(nullptr);
}

void Minimap::draw() {
	if (!active || !playerRealm) return;

	pair size = zoom * pair(CHUNK_SIZE, CHUNK_SIZE);
	pair origin = worldToMap(vec(CHUNK_SIZE - 1, CHUNK_SIZE - 1) / 2);

	for (auto& [pos,c] : playerRealm->chunkManager.chunks) {
		
		pair offset = pos * CHUNK_SIZE * zoom;
		if (c.mapTexture) {
			TextureManager::drawTexture(c.mapTexture, nullptr, {0,0}, size, origin + offset, size);
		}

		if (chunkOverlay) {
			float t = float(c.stage)/ChunkStage::MAX;
			TextureManager::drawRect(origin + offset, size, {uint8_t(255 * (1-t)), uint8_t(255 * t), 0, 200}, true, true);
		}

		if (tempOverlay && c.tempTexture) {
			TextureStyle style;
			style.alpha = 50;
			TextureManager::drawTexture(c.tempTexture, nullptr, {0,0}, size, origin + offset, size, style);
		}

	}

	TextureManager::drawCirc(Window::instance->size/2, 5);

	widget->draw();

	// SDL_SetRenderDrawColor(Window::instance->renderer, 255, 0, 0, 100);

	// pair chunkNumber = size / CHUNK_SIZE;
	// for (int x = 0; x <= chunkNumber.x; x++) {
	// 	int xx = x * CHUNK_SIZE + offset.x;
	// 	pair start = worldToMap(vec(xx - 0.5f, -1 + offset.y));
	// 	pair end = worldToMap(vec(xx - 0.5f, size.y + 0.5f + offset.y));
	// 	SDL_RenderDrawLine(Window::instance->renderer, start.x, start.y, end.x, end.y);
	// }

	// for (int y = 0; y <= chunkNumber.y; y++) {
	// 	int yy = y * CHUNK_SIZE + offset.y;
	// 	pair start = worldToMap(vec(-1 + offset.x, yy - 0.5f));
	// 	pair end = worldToMap(vec(size.y + 0.5f + offset.x, yy - 0.5f));
	// 	SDL_RenderDrawLine(Window::instance->renderer, start.x, start.y, end.x, end.y);
	// }

	// for (int x = 0; x < chunkNumber.x; x++) {
	// 	for (int y = 0; y < chunkNumber.y; y++) {
	// 		pair chunk(x, y);
	// 		pair position = CHUNK_SIZE * chunk + offset;
		
			

	// 	}
	// }

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
	return Window::instance->size/2 + vec::round(zoom * (position - playerPosition));
}

vec Minimap::mapToWorld(pair position) {
	return playerPosition + vec(position - Window::instance->size/2) / zoom;
}
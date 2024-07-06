
#pragma once
#include "TextureManager.hpp"
#include "TextManager.hpp"
#include "Window.hpp"
#include "Events.hpp"

#include "Chunk.hpp"

class Realm;


class Minimap {
public:
	bool handleEvent(InputEvent event);
	void refresh(Realm* realm);
	void update();
	void draw(Realm* realm, vec playerPos);

private:
	pair worldToMap(vec position);

	SDL_Texture* texture;
	pair offset;
	pair size;

	int zoom = 1;
	bool active = false;

	friend class Realm;
};
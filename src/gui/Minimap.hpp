
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
	void update(vec PlayerPos, Realm* realm);
	void draw();

private:
	pair worldToMap(vec position);
	vec mapToWorld(pair position);

	vec playerPosition;
	Realm* playerRealm;

	int zoom = 1;
	bool active = false;

	friend class Realm;
};
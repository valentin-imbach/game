
#pragma once
#include "TextureManager.hpp"
#include "TextManager.hpp"
#include "Window.hpp"
#include "Events.hpp"

#include "Chunk.hpp"
#include "GuiElement.hpp"

class Realm;


class Minimap {
public:
	Minimap();
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

	bool chunkOverlay = false;
	bool tempOverlay = false;
	std::unique_ptr<Widget> widget;

	friend class Realm;
};
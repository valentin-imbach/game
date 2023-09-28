
#pragma once
#include <vector>
#include "Components.hpp"
#include "EntityFactory.hpp"
#include "Events.hpp"
#include "SDL2/SDL_events.h"
#include "TextManager.hpp"
#include "Window.hpp"
#include "utils.hpp"
#include "TextureManager.hpp"
#include "World.hpp"

class Game;

class Console {
public:
	Console(Game* game);

	void draw();
	bool handleEvent(InputEvent event);
	bool active = false;

private:
	Game* game;
	std::string input;
	std::vector<std::string> history;
	uint index = 0;

	bool execute(std::string input);
};
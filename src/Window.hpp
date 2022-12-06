
#pragma once
#include <SDL2/SDL.h>
#include "utils/utils.hpp"

class Window {
public:
	Window(const char* title, pair size, bool fullscreen = false);

	void clear();
	void update();
	void draw();

	const char* title;
	SDL_Window* sdl_window;
	SDL_Renderer* renderer;

	pair size;
	pair mousePosition;

	const Uint8* keyState;

	static Window* instance;
};
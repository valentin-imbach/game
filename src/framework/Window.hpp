
#pragma once
#include <SDL2/SDL.h>
#include "utils.hpp"

class Window {
public:
	Window(const char* title, pair size, bool fullscreen = false);

	void clear();
	void update();
	void draw();
	void cleanup();

	const char* title;
	SDL_Window* window;
	SDL_Renderer* renderer;

	pair size;
	pair mousePosition;

	std::filesystem::path root;

	static Window* instance;
};
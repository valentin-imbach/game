
#include "Window.hpp"

Window* Window::instance = nullptr;

Window::Window(const char* title, pair size, bool fullscreen) : title(title), size(size) {

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
		ERROR("Failed to inizialize SDL", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	LOG("SDL initialized");
	
	root = std::filesystem::path(SDL_GetBasePath()).parent_path().parent_path();
	LOG("Root path is", root.string());

	int flags = fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE;
	window = SDL_CreateWindow(title, size.x, size.y, flags);
	if (!window) {
		ERROR("Failed to create Window");
		exit(EXIT_FAILURE);
	}
	SDL_StartTextInput(window);
	LOG("Window created");

	SDL_SetWindowMinimumSize(window, 1024, 640);
	renderer = SDL_CreateRenderer(window, NULL);
	if (!renderer) {
		ERROR("Failed to create Renderer");
		exit(EXIT_FAILURE);
	}
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	LOG("Renderer created");

	instance = this;
}

void Window::cleanup() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

void Window::clear() {
	SDL_SetRenderDrawColor(Window::renderer, 20, 20, 20, 255);
	SDL_RenderClear(Window::renderer);
}

void Window::update() {
	SDL_GetWindowSize(window, &size.x, &size.y);
	SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
}

void Window::draw() {
	SDL_RenderPresent(Window::renderer);
}
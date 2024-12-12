
#include "Window.hpp"

Window* Window::instance = nullptr;

Window::Window(const char* title, pair size, bool fullscreen) : title(title), size(size) {

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
		ERROR("Failed to inizialize SDL:", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	LOG("SDL initialized");
	
	root = std::filesystem::path(SDL_GetBasePath()).parent_path().parent_path();
	LOG("Root path is", root.string());

	Uint64 flags = fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE;

	if (!SDL_CreateWindowAndRenderer(title, size.x, size.y, flags, &window, &renderer)) {
		ERROR("Failed to create Window: or Renderer:", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_SetWindowMinimumSize(window, 1024, 640);
	SDL_StartTextInput(window);
	LOG("Window and Renderer created");

	instance = this;
}

void Window::cleanup() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

void Window::clear() {
	SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
	SDL_RenderClear(renderer);
}

void Window::update() {
	SDL_GetWindowSize(window, &size.x, &size.y);
	mouseState = SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
	keyState = SDL_GetKeyboardState(NULL);
}

void Window::draw() {
	SDL_RenderPresent(Window::renderer);
}
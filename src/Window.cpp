
#include "Window.hpp"
#include "utils/logger.hpp"

Window::Window(const char* title, pair size, bool fullscreen) : title(title), size(size) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        ERROR("Failed to inizialize SDL", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    LOG("SDL initialized");

    int flags = fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_RESIZABLE;
    sdl_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.x, size.y, flags);
    if (!sdl_window) {
        ERROR("Failed to create Window");
        exit(EXIT_FAILURE);
    }
    LOG("Window created");

    SDL_SetWindowMinimumSize(sdl_window, 1024, 640);
    renderer = SDL_CreateRenderer(sdl_window, -1, 0);
    if (!renderer) {
        ERROR("Failed to create Renderer");
        exit(EXIT_FAILURE);
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    LOG("Renderer created");
}

void Window::update() {
    SDL_GetWindowSize(sdl_window, &size.x, &size.y);
}
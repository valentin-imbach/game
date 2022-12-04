
#pragma once
#include <SDL2/SDL.h>
#include "utils/pair.hpp"

class Window {
public:
    Window(const char* title, pair size, bool fullscreen = false);

    void clear();
    void update();

    const char* title;
    SDL_Window* sdl_window;
    SDL_Renderer* renderer;

    pair size;

    static Window* instance;
};
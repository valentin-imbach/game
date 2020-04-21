//
//  Window.hpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-19.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#pragma once

#define MAX_FPS 60

#include <SDL2/SDL.h>
#include "tools.h"
#include "TextManager.hpp"
#include "TextureManager.hpp"

class Window {
public:
    
    Window(const char *title, int width, int height, bool fullscreen = false);
    ~Window();
    
    static SDL_Renderer* renderer;
    
    void update();
    inline bool isRunning() { return running; }
    
    static const Uint8* keys;
    static v(SDL_Event) events;
    static pint size;
    static pint mousePos;
    static int FPS;
    
private:
    void keyPressed(SDL_KeyboardEvent& k);
    void mousePressed(SDL_MouseButtonEvent& b);
    Uint32 lastFrame;
    int limitFrameRate(int fps);
    void handleEvents();
    SDL_Event event;
    SDL_Window* win;
    bool running;
};




//
//  Window.hpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-19.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#pragma once
#include "tools.h"

class Window {
public:
    Window(const char *title, int width, int height, bool fullscreen = false);
    static SDL_Renderer* renderer;
    
    void update();

    static bool running;
    static const Uint8* keys;
    static v(SDL_Event) events;
    static pair<int> size;
    static pair<int> mousePos;
    static int MAX_FPS;
    static int FPS;
    
private:
    void keyPressed(SDL_KeyboardEvent& k);
    void mousePressed(SDL_MouseButtonEvent& b);
    int limitFrameRate(int fps);
    Uint32 lastFrame;
    void handleEvents();
    SDL_Event event;
    SDL_Window* win;

};





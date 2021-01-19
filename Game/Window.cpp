//
//  Window.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-19.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "tools.h"
#include "Window.hpp"

SDL_Renderer* Window::renderer = nullptr;
const Uint8* Window::keys = nullptr;
v(SDL_Event) Window::events = v(SDL_Event)();
int Window::FPS = NULL;
int Window::MAX_FPS = 60;
pair<int> Window::size;
pair<int> Window::mousePos;
bool Window::running = true;

Window::Window(const char *title, int width, int height, bool fullscreen) {
    lastFrame = SDL_GetTicks();
    size = {width,height};
    
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        LOG("SDL initialized");
        int flags = fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_RESIZABLE;
        win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
        if (win) {
            LOG("Window created");
            SDL_SetWindowMinimumSize(win, 1024, 640);
            
            renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);
            if (renderer) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                LOG("Renderer created");
                running = true;
                
            } else ERROR("Failed to create Renderer");
        } else ERROR("Failed to create Window");
    } else ERROR("Failed to inizialize SDL");
    
    update();
};

void Window::update() {
    SDL_GetWindowSize(win, &size.X, &size.Y);
    keys = SDL_GetKeyboardState(NULL);
    SDL_GetMouseState(&mousePos.X, &mousePos.Y);
    handleEvents();
    FPS = limitFrameRate(MAX_FPS);
};

void Window::handleEvents() {
    events.clear();
    
    SDL_Event resetEvent = SDL_Event();
    resetEvent.type = SDL_USEREVENT;
    resetEvent.user.code = (int)EventCode::RESET;
    events.push_back(resetEvent);
    
    while (SDL_PollEvent(&event)) {
        events.push_back(event);
        if (event.type == SDL_QUIT) {
            LOG("Window closed");
            running = false;
            break;
        }
    }
    
    SDL_Event keyEvent = SDL_Event();
    keyEvent.type = SDL_USEREVENT;
    keyEvent.user.code = (int)EventCode::KEYSTATE;
    events.push_back(keyEvent);
    
    SDL_Event hoverEvent = SDL_Event();
    hoverEvent.type = SDL_USEREVENT;
    hoverEvent.user.code = (int)EventCode::HOVER;
    events.push_back(hoverEvent);
}

int Window::limitFrameRate(int fps) {
    int delay = ceil(1000.0/fps);
    Uint32 ticks = SDL_GetTicks();
    Uint32 past = ticks-lastFrame;
    if (past < delay) {
        SDL_Delay(delay-past);
        past = delay;
    }
    lastFrame = SDL_GetTicks();
    return 1000.0/past;
};




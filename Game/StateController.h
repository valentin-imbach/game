//
//  StateController.h
//  Game
//
//  Created by Valentin Imbach on 28.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

enum GameState {
    RUNNING,
    PAUSED,
    LOADING,
    MENU
};

struct StateController {
    
    GameState state = LOADING;
    
    bool handleEvent(SDL_Event event) {
        if (event.key.repeat) { return false; }
        if (state == RUNNING) {
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                state = PAUSED; return true;
            }
        }
        if (state == PAUSED) {
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                state = RUNNING;
                return true;
            }
        }
        return false;
    }
};

//
//  StateController.h
//  Game
//
//  Created by Valentin Imbach on 28.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

enum class GameState {
    RUNNING,
    PAUSED,
    LOADING,
    MAIN_MENU,
    STARTING
};

struct StateController {
    
    GameState state = GameState::STARTING;
    
    bool handleEvent(SDL_Event event) {
        if (event.key.repeat) { return false; }
        
        if (state == GameState::RUNNING) {
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                state = GameState::PAUSED;
                return true;
            }
        }
        
        if (state == GameState::PAUSED) {
            if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                state = GameState::RUNNING;
                return true;
            }
        }
        return false;
    }
};

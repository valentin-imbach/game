//
//  PlayerInputComponent.h
//  Game
//
//  Created by Valentin Imbach on 27.03.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "Components.h"
#include "Window.hpp"

#include "../GuiSystem/Guis.h"

class PlayerInputComponent : public Component {
    
private:
    PositionComponent *positionComponent;
    DirectionComponent *directionComponent;
    InventoryComponent *inventoryComponent;
    pair<float> oldPos;
    
public:
    
    int ticks = 0;
    bool walking = false;
    
    void init() override {
        positionComponent = &entity -> addComponent<PositionComponent>();
        directionComponent = &entity -> addComponent<DirectionComponent>();
        inventoryComponent = &entity -> getComponent<InventoryComponent>();
    }
    
    void update() override {
        
        bool walk = false;
        int dir = -1;
        
        pair<float> offset;
        
        float speed = 0.07;
        
        if (Window::keys[SDL_SCANCODE_W]) {
            offset.Y = -speed;
            dir = NORTH;
            walk = true;
        } else if (Window::keys[SDL_SCANCODE_S]) {
            offset.Y = speed;
            dir = SOUTH;
            walk = true;
        }
        
        if (Window::keys[SDL_SCANCODE_A]) {
            offset.X = -speed;
            if (dir == -1) {
                dir = WEST;
            } else if (dir == NORTH) {
                dir = NWEST;
            } else {
                dir = SWEST;
            }
            walk = true;
        } else if (Window::keys[SDL_SCANCODE_D]) {
            offset.X = speed;
            if (dir == -1) {
                dir = EAST;
            } else if (dir == NORTH) {
                dir = NEAST;
            } else {
                dir = SEAST;
            }
            walk = true;
        }
        
        if (offset.X != 0 && offset.Y != 0) {
            offset.X /= sqrt(2);
            offset.Y /= sqrt(2);
        }
        
        oldPos = positionComponent -> pos;
        positionComponent -> pos += offset;
        
        if (dir != -1) {
            directionComponent -> direction = (Direction)dir;
        }
        
        if (walk) {
            ticks += 1;
        } else {
            ticks = 0;
        }
    }
    
    void setBack() {
        positionComponent -> pos = oldPos;
    }
    
    bool handleEvent(SDL_Event event) override {
        
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_E) {
                GuiManager::addGui(new InventoryGui(inventoryComponent,Window::size.X/2 ,Window::size.Y/2));
                return true;
            } else if (event.key.keysym.scancode == SDL_SCANCODE_F) {
                
            }
        }
        
        return false;
    }
    
};

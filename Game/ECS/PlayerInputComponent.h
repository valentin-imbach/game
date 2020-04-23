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
        
        float xoff = 0;
        float yoff = 0;
        
        float speed = 0.07;
        
        if (Window::keys[SDL_SCANCODE_W]) {
            yoff = -speed;
            dir = NORTH;
            walk = true;
        } else if (Window::keys[SDL_SCANCODE_S]) {
            yoff = speed;
            dir = SOUTH;
            walk = true;
        }
        
        if (Window::keys[SDL_SCANCODE_A]) {
            xoff = -speed;
            if (dir == -1) {
                dir = WEST;
            } else if (dir == NORTH) {
                dir = NWEST;
            } else {
                dir = SWEST;
            }
            walk = true;
        } else if (Window::keys[SDL_SCANCODE_D]) {
            xoff = speed;
            if (dir == -1) {
                dir = EAST;
            } else if (dir == NORTH) {
                dir = NEAST;
            } else {
                dir = SEAST;
            }
            walk = true;
        }
        
        if (xoff != 0 && yoff != 0) {
            xoff /= sqrt(2);
            yoff /= sqrt(2);
        }
        
        positionComponent -> pos.X += xoff;
        positionComponent -> pos.Y += yoff;
        
        if (dir != -1) {
            directionComponent -> direction = (Direction)dir;
        }
        
        if (walk) {
            ticks += 1;
        } else {
            ticks = 0;
        }
    }
    
    bool handleEvent(SDL_Event event) override {
        
        if (event.type == SDL_KEYDOWN) {
            if (!event.key.repeat) {
                if (event.key.keysym.scancode == SDL_SCANCODE_E) {
                    LOG("E");
                    GuiManager::addGui(new InventoryGui(inventoryComponent,Window::size.X/2 ,Window::size.Y/2));
                    return true;
                }
            }
        }
        
        return false;
    }
    
};

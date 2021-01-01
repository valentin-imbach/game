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

class PlayerInputComponent : public Component {
    
private:
    PositionComponent *positionComponent;
    DirectionComponent *directionComponent;
    InventoryComponent *inventoryComponent;
    CollisionComponent *collisionComponent;
    pair<float> oldPos;
    
public:
    static ComponentType componentType;
    bool god = false;
    int ticks = 0;
    bool walking = false;
    bool keystate = false;
    
    void init() override {
        collisionComponent = entity -> getComponent<CollisionComponent>();
        positionComponent = entity -> getComponent<PositionComponent>();
        directionComponent = entity -> getComponent<DirectionComponent>();
        inventoryComponent = entity -> getComponent<InventoryComponent>();
    }
    
    void update() override {
        
        if (!keystate) { return; }
        
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
        
        if (checkCollision(positionComponent -> position) || !checkCollision(positionComponent -> position + offset) || god) {
            positionComponent -> position += offset;
        }
        
        if (dir != -1) {
            directionComponent -> direction = (Direction)dir;
        }
        
        if (walk) {
            ticks += 1;
        } else {
            ticks = 0;
        }
    }
    
    bool checkCollision(pair<float> pos) {
        pair<int> p1 = (pos - collisionComponent -> offset).rounded();
        pair<int> p2 = (pos + collisionComponent -> collider.size - collisionComponent -> offset).rounded();
        
        for (int i = 0; i < p2.X-p1.X+1; i++) {
            for (int j = 0; j < p2.Y-p1.Y+1; j++) {
                Entity* str = entity -> manager -> gridEntities[p1.X+i][p1.Y+j];
                if (str != nullptr && str -> getComponent<GridComponent>() -> solid) {
                    return true;
                }
            }
        }
        
        return false;
    }
    
    bool handleEvent(SDL_Event event) override {
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_E) {
                MessageManager::notify(InventoryMessage());
                return true;
            } else if (event.key.keysym.scancode == SDL_SCANCODE_Q) {
                MessageManager::notify(ItemThrowMessage());
            }
        } else if (event.type == SDL_USEREVENT && event.user.code == (int)EventCode::KEYSTATE) {
            keystate = true;
            return true;
        } else if (event.type == SDL_USEREVENT && event.user.code == (int)EventCode::RESET) {
            keystate = false;
            return false;
        }
        
        return false;
    }
    
    Component* create() override {
        return new PlayerInputComponent();
    }
    
};

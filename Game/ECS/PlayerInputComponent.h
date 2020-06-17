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
    PlayerGuiComponent *playerGuiComponent;
    CollisionComponent *collisionComponent;
    pair<float> oldPos;
    
public:
    static ComponentType componentType;
    bool god = false;
    int ticks = 0;
    bool walking = false;
    
    PlayerInputComponent() {}
    
    void init() override {
        collisionComponent = entity -> getComponent<CollisionComponent>();
        positionComponent = entity -> getComponent<PositionComponent>();
        directionComponent = entity -> getComponent<DirectionComponent>();
        inventoryComponent = entity -> getComponent<InventoryComponent>();
        playerGuiComponent = entity -> getComponent<PlayerGuiComponent>();
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
        
        return !(entity -> manager -> isFree(p1.X,p1.Y,p2.X-p1.X+1,p2.Y-p1.Y+1));
    }
    
    bool handleEvent(SDL_Event event) override {
        
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_E) {
                playerGuiComponent -> makeInventoryGui(Window::size/2);
                return true;
            } else if (event.key.keysym.scancode == SDL_SCANCODE_Q) {
                int s = playerGuiComponent -> selected;
                Item* item = inventoryComponent -> containers[s][0].item;
                if (item != nullptr) {
                    Entity* e = entity -> manager -> addEntity();
                    e -> addComponent<PositionComponent>(positionComponent -> position + dirs2[directionComponent -> direction]);
                    e -> addComponent<CollisionComponent>();
                    e -> addComponent<ItemComponent>(item);
                    inventoryComponent -> containers[s][0].item = nullptr;
                }
            }
        }
        
        return false;
    }
    
    Component* create() override {
        return new PlayerInputComponent();
    }
    
};

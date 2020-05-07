//
//  HotbarGui.h
//  Game
//
//  Created by Valentin Imbach on 25.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

#include "GuiManager.hpp"
#include "../ECS/InventoryComponent.h"
#include "../TextureManager.hpp"

class HotbarGui : public Gui {
    
private:
    InventoryComponent* inventory;
    int offset = 57;
    int scale = 48;
    
public:
    
    int selected;
    
    HotbarGui(InventoryComponent* inv, int x, int y) {
        inventory = inv;
        size = {(inv -> size.X-1) * offset + scale, scale};
        position = {x,y};
        setSelection(0);
    }
    
    void render() override {
        TextureManager::drawTexture(TextureManager::hotbarTexture, 0, 0, 178, 26, position.X, position.Y, 534, 78, true);
        for (int i = 0; i < inventory -> size.X; i++) {
            if (inventory -> itemSlots[i][0].item != nullptr) {
                inventory -> itemSlots[i][0].item -> render(position.X-size.X/2+i*offset+scale/2,position.Y-size.Y/2+scale/2,scale);
            }
        }
        SDL_Rect rect;
        rect.x = position.X-size.X/2+selected*offset-5;
        rect.y = position.Y-size.Y/2-5;
        rect.w = scale+10;
        rect.h = scale+10;
        SDL_RenderDrawRect(Window::renderer, &rect);
    }
    
    void setSelection(int x) {
        selected = x;
        GuiManager::hotbarSlot = &(inventory -> itemSlots[x][0]);
    }
    
    bool handleEvent(SDL_Event event) override {
        if (event.type == SDL_MOUSEWHEEL) {
            setSelection((selected - sign(event.wheel.y) + inventory -> size.X) % (inventory -> size.X));
            return true;
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_1) { selected = 0; return true; }
            if (event.key.keysym.scancode == SDL_SCANCODE_2) { selected = 1; return true; }
            if (event.key.keysym.scancode == SDL_SCANCODE_3) { selected = 2; return true; }
            if (event.key.keysym.scancode == SDL_SCANCODE_4) { selected = 3; return true; }
            if (event.key.keysym.scancode == SDL_SCANCODE_5) { selected = 4; return true; }
            if (event.key.keysym.scancode == SDL_SCANCODE_6) { selected = 5; return true; }
            if (event.key.keysym.scancode == SDL_SCANCODE_7) { selected = 6; return true; }
            if (event.key.keysym.scancode == SDL_SCANCODE_8) { selected = 7; return true; }
            if (event.key.keysym.scancode == SDL_SCANCODE_9) { selected = 8; return true; }
        }
        return false;
    }
    
};

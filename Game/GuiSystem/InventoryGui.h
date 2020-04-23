//
//  InventoryGui.h
//  Game
//
//  Created by Valentin Imbach on 23.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "tools.h"
#include "GuiManager.hpp"
#include "../ECS/InventoryComponent.h"

class InventoryGui : public Gui {
    
private:
    InventoryComponent* inventory;
    int offset = 60;
    int scale = 48;
    
public:
    InventoryGui(InventoryComponent* inv, int x, int y) {
        inventory = inv;
        size = {inv -> size.X * offset + scale,inv -> size.Y * offset + scale};
        position = {x,y};
    }
    
    void render() override {
        
        for (int i = 0; i < inventory -> size.X; i++) {
            for (int j = 0; j < inventory -> size.Y; j++) {
                if (inventory -> items[i][j] != nullptr) {
                    inventory -> items[i][j] -> render(position.X-size.X/2+i*offset,position.Y-size.Y/2+j*offset,scale);
                }
            }
        }
    }
    
    void update() override {}
    
    bool handleEvent(SDL_Event event) override {
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_E) {
                alive = false;
                return true;
            }
        }
        return false;
    }
    
};

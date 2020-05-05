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
#include "../TextureManager.hpp"

class InventoryGui : public Gui {
    
private:
    InventoryComponent* inventory;
    int offset = 57;
    int scale = 48;
    
public:
    InventoryGui(InventoryComponent* inv, int x, int y) {
        inventory = inv;
        size = {(inv -> size.X-1) * offset + scale, (inv -> size.Y-1) * offset + scale};
        position = {x,y};
    }
    
    void render() override {
        
        TextureManager::drawTexture(TextureManager::inventoryTexture, 0, 0, 178, 102, position.X, position.Y, 534, 306, true);
        for (int i = 0; i < inventory -> size.X; i++) {
            for (int j = 0; j < inventory -> size.Y; j++) {
                if (inventory -> itemSlots[i][j].item != nullptr) {
                    inventory -> itemSlots[i][j].item -> render(position.X-size.X/2+i*offset+scale/2,position.Y-size.Y/2+j*offset+scale/2,scale);
                }
            }
        }
    }
    
    bool handleEvent(SDL_Event event) override {
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_E) {
                alive = false;
                inventory -> addItem(GuiManager::mouseSlot -> item);
                GuiManager::mouseSlot -> item = nullptr;
                return true;
            }
        }
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (check(Window::mousePos)) {
                
                pair<int> relPos = Window::mousePos-position+size/2;
                pair<int> slot = relPos/offset;
                ItemSlot* itemSlot = &(inventory -> itemSlots[slot.X][slot.Y]);
                
                if (event.button.button == SDL_BUTTON_LEFT) {
                    
                    if (inventory -> itemSlots[slot.X][slot.Y].similar(GuiManager::mouseSlot -> item)) {
                        GuiManager::mouseSlot -> item = inventory -> itemSlots[slot.X][slot.Y].addFullItem(GuiManager::mouseSlot -> item);
                    } else {
                        Item* temp = itemSlot -> item;
                        itemSlot -> item = GuiManager::mouseSlot -> item;
                        GuiManager::mouseSlot -> item = temp;
                    }
                    return true;
                    
                } else if (event.button.button == SDL_BUTTON_RIGHT) {
                    if (GuiManager::mouseSlot -> item == nullptr) {
                        GuiManager::mouseSlot -> item = itemSlot -> takeHalf();
                    } else {
                        GuiManager::mouseSlot -> item = itemSlot -> addOneItem(GuiManager::mouseSlot -> item);
                    }
                }
                return true;
            }
        }
        return false;
    }
    
};

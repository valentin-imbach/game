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
                if (inventory -> items[i][j] != nullptr) {
                    inventory -> items[i][j] -> render(position.X-size.X/2+i*offset,position.Y-size.Y/2+j*offset,scale);
                }
            }
        }
    }
    
    bool handleEvent(SDL_Event event) override {
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_E) {
                alive = false;
                inventory -> addItem(GuiManager::mouseItem);
                GuiManager::mouseItem = nullptr;
                return true;
            }
        }
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (check(Window::mousePos)) {
                
                pair<int> relPos = Window::mousePos-position+size/2;
                pair<int> slot = relPos/offset;
                Item* item = inventory -> items[slot.X][slot.Y];
                
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (GuiManager::mouseItem != nullptr && item != nullptr) {
                        if (GuiManager::mouseItem -> stack  && item -> stack) {
                            if (GuiManager::mouseItem -> type == item -> type) {
                                item -> count += GuiManager::mouseItem -> count;
                                delete GuiManager::mouseItem;
                                GuiManager::mouseItem = nullptr;
                                return true;
                            }
                        }
                    }
                    Item* temp = item;
                    inventory -> items[slot.X][slot.Y] = GuiManager::mouseItem;
                    GuiManager::mouseItem = temp;
                    return true;
                } else if (event.button.button == SDL_BUTTON_RIGHT) {
                    if (GuiManager::mouseItem != nullptr && item != nullptr) {
                        if (GuiManager::mouseItem -> stack && item -> stack) {
                            if (GuiManager::mouseItem -> type == item -> type) {
                                item -> count += 1;
                                GuiManager::mouseItem -> count -= 1;
                                if (GuiManager::mouseItem -> count == 0) {
                                    delete GuiManager::mouseItem;
                                    GuiManager::mouseItem = nullptr;
                                }
                                return true;
                            }
                        }
                    } else if (GuiManager::mouseItem != nullptr) {
                        if (GuiManager::mouseItem -> stack) {
                            inventory -> items[slot.X][slot.Y] = new ItemStack(GuiManager::mouseItem -> type, 1);
                            GuiManager::mouseItem -> count -= 1;
                            if (GuiManager::mouseItem -> count == 0) {
                                delete GuiManager::mouseItem;
                                GuiManager::mouseItem = nullptr;
                            }
                            return true;
                        }
                    } else if (item != nullptr) {
                        if (item -> stack) {
                            int count = (item -> count + 1)/2;
                            GuiManager::mouseItem = new ItemStack(item -> type, count);
                            item -> count -= count;
                            if (item -> count == 0) {
                                delete item;
                                inventory -> items[slot.X][slot.Y] = nullptr;
                            }
                            return true;
                        }
                    }
                    
                }
                return true;
            }
        }
        return false;
    }
    
};

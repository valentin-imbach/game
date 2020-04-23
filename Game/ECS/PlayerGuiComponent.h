//
//  PlayerGuiComponent.h
//  Game
//
//  Created by Valentin Imbach on 21.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "Components.h"

class PlayerGuiComponent : public GuiComponent {
private:
    InventoryComponent* inventoryComponent;
public:
    void init() override {
        inventoryComponent = &entity -> addComponent<InventoryComponent>(9,4);
    }
    
    PlayerGuiComponent() {
        size = {200,200};
        position = {300,300};
    }

    void guiRender() override {
        int x = 200;
        int y = 200;
        int d = 60;
        int scale = 3;
            
        for (int i = 0; i < inventoryComponent -> size.X; i++) {
            for (int j = 0; j < inventoryComponent -> size.Y; j++) {
                if (inventoryComponent -> items[i][j] != nullptr) {
                    inventoryComponent -> items[i][j] -> render(x+i*d,y+j*d,scale);
                }
            }
        }
    }
    
    void guiUpdate() override {}
    
    bool guiHandleEvent(SDL_Event event) override {
        return false;
    }
    
};

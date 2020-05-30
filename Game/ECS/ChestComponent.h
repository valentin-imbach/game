//
//  ChestComponent.hpp
//  Game
//
//  Created by Valentin Imbach on 18.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "../Item.h"
#include "../Gui/GuiManager.hpp"
#include "Components.h"

class ChestComponent : public Component {
public:
    InventoryComponent* inventoryComponent;
    pair<int> size = {5,3};
    
    ChestComponent(Entity* entity, pair<int> pos) {
        inventoryComponent = entity -> addComponent<InventoryComponent>(5,3);
        entity -> addComponent<GridComponent>(pos);
        entity -> addComponent<SpriteComponent>("chest.png");
    }
    
    bool handleEvent(SDL_Event event) override {
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_RIGHT) {
                GuiElement* inv = entity -> manager -> player -> getComponent<PlayerGuiComponent>() -> makeInventoryGui(Window::size/2 + pair<int>(0,100));
                makeGui(Window::size/2 - pair<int>(0,180), inv);
                return true;
            }
        }
        return false;
    }
    
    GuiElement* makeGui(pair<int> pos, GuiElement* link) {
        GuiElement* gui = new Widget(pos,{306, 192},TextureManager::getTexture("chestGui.png"), link);
        for (int i = 0; i < size.X; i++) {
            for (int j = 0; j < size.Y; j++) {
                gui -> addGuiElement(new ItemSlot({39+i*57,39+j*57},&(inventoryComponent -> containers[i][j])));
            }
        }
        GuiManager::manager -> addGuiElement(gui);
        return gui;
    }
    
};

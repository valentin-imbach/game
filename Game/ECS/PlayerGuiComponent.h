//
//  PlayerGuiComponent.h
//  Game
//
//  Created by Valentin Imbach on 25.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

#include "Components.h"
#include "Window.hpp"
#include "../Gui/GuiManager.hpp"

class PlayerGuiComponent : public Component {
public:
    InventoryComponent *inventoryComponent;
    int selected = 0;
    
    PlayerGuiComponent(Entity* entity) {
        inventoryComponent = entity -> getComponent<InventoryComponent>();
        makeHotbarGui();
    }
    
    GuiElement* makeHotbarGui() {
        v(ItemContainer*) vec;
        for (int i = 0; i < inventoryComponent -> size.X; i++) {
            vec.push_back(&(inventoryComponent -> containers[i][0]));
        }
        
        GuiElement* hotbar = new Hotbar(vec, &selected);
        GuiManager::manager -> addGuiElement(hotbar);
        return hotbar;
    }
    
    GuiElement* makeInventoryGui(pair<int> pos) {
        GuiElement* gui = new Widget(pos,{534, 306},TextureManager::inventoryTexture);
        for (int i = 0; i < inventoryComponent -> size.X; i++) {
            for (int j = 0; j < inventoryComponent -> size.Y; j++) {
                gui -> addGuiElement(new ItemSlot({39+i*57,39+j*57},&(inventoryComponent -> containers[i][j])));
            }
        }
        GuiManager::manager -> addGuiElement(gui);
        return gui;
    }
    
    ItemContainer* getSelectedItem() {
        return &(inventoryComponent -> containers[selected][0]);
    }
    
};

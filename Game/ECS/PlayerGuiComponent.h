//
//  PlayerGuiComponent.h
//  Game
//
//  Created by Valentin Imbach on 25.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

#include "Components.h"
#include "../GuiSystem/Guis.h"
#include "Window.hpp"
#include "../Gui/GuiManager.hpp"

class PlayerGuiComponent : public Component {
public:
    InventoryComponent *inventoryComponent;
    HotbarGui* hotbarGui;
    
    PlayerGuiComponent(Entity* entity) {
        inventoryComponent = entity -> getComponent<InventoryComponent>();
        hotbarGui = new HotbarGui(inventoryComponent, Window::size.X/2, 50);
        //GuiManager::addGui(hotbarGui);
        
        GuiElement* hotbar = new GuiElement({Window::size.X/2,60},{534, 78},TextureManager::hotbarTexture);
        for (int i = 0; i < inventoryComponent -> size.X; i++) {
            hotbar -> addGuiElement(new ItemSlot({39+i*57,39},&(inventoryComponent -> itemSlots[i][0])));
        }
        GuiManager2::manager -> addGuiElement(hotbar);

    }
};

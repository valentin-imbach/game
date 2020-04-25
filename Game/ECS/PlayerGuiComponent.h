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

class PlayerGuiComponent : public Component {
public:
    InventoryComponent *inventoryComponent;
    HotbarGui* hotbarGui;
    
    void init() override {
        inventoryComponent = &entity -> getComponent<InventoryComponent>();
        hotbarGui = new HotbarGui(inventoryComponent, Window::size.X/2, 50);
        GuiManager::addGui(hotbarGui);
    }
};

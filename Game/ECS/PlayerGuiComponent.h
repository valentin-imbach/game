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

#include "../MessagingSystem.hpp"

class PlayerGuiComponent : public Component {
public:
    static ComponentType componentType;
    InventoryComponent *inventoryComponent;
    HealthComponent *healthComponent;
    int selected = 0;
    
    void init() override {
        inventoryComponent = entity -> getComponent<InventoryComponent>();
        healthComponent = entity -> getComponent<HealthComponent>();
        GuiManager::manager -> addGuiElement(makeHotbarGui());
        GuiManager::manager -> addGuiElement(makeHealthGui());
    }
    
    GuiElement* makeHotbarGui() {
        v(ItemContainer*) vec;
        for (int i = 0; i < inventoryComponent -> size.X; i++)
            vec.push_back(&(inventoryComponent -> containers[i][0]));
        GuiElement* hotbar = new Hotbar(vec, &selected);
        return hotbar;
    }
    
    GuiElement* makeHealthGui() {
        GuiElement* gui = new HealthBar(&(healthComponent -> health));
        return gui;
    }
    
    GuiElement* makeInventoryGui(pair<int> pos) {
        GuiElement* gui = new Widget(pos,{534, 306},TextureManager::getTexture("inventory.png"));
        for (int i = 0; i < inventoryComponent -> size.X; i++) {
            for (int j = 0; j < inventoryComponent -> size.Y; j++) {
                gui -> addGuiElement(new ItemSlot({39+i*57,39+j*57},&(inventoryComponent -> containers[i][j])));
            }
        }
        GuiManager::manager -> addGuiElement(gui);
        MessageManager::notify(Message(MessageType::INVENTORY));
        return gui;
    }
    
    ItemContainer* getSelectedItem() {
        return &(inventoryComponent -> containers[selected][0]);
    }
    
    Component* create() override {
        return new PlayerGuiComponent();
    }
    
};

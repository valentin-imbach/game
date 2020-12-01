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

class PlayerGuiComponent : public Component, public Observer {
public:
    static ComponentType componentType;
    InventoryComponent *inventoryComponent;
    HealthComponent *healthComponent;
    PositionComponent *positionComponent;
    DirectionComponent *directionComponent;
    int selected = 0;
    
    void init() override {
        inventoryComponent = entity -> getComponent<InventoryComponent>();
        healthComponent = entity -> getComponent<HealthComponent>();
        directionComponent = entity -> getComponent<DirectionComponent>();
        positionComponent = entity -> getComponent<PositionComponent>();
        GuiManager::manager -> addGuiElement(makeHotbarGui());
        GuiManager::manager -> addGuiElement(makeHealthGui());
        subscribe(MessageType::INVENTORY);
        subscribe(MessageType::ITEM_THROW);
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
        return gui;
    }
    
    bool onMessage(const Message &message) override {
        if (message.type == MessageType::INVENTORY) {
            makeInventoryGui(Window::size/2);
            return true;
        } else if (message.type == MessageType::ITEM_THROW) {
            Item* item = inventoryComponent -> containers[selected][0].item;
            if (item != nullptr) {
                MessageManager::notify(SpawnItemMessage(item,positionComponent -> position + dirs2[directionComponent -> direction]));
                inventoryComponent -> containers[selected][0].item = nullptr;
            }
        }
        return false;
    }
    
    ItemContainer* getSelectedItem() {
        return &(inventoryComponent -> containers[selected][0]);
    }
    
    Component* create() override {
        return new PlayerGuiComponent();
    }
    
};

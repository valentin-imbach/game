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

#define RADIUS 3

class PlayerGuiComponent : public Component {
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
        entity -> subscribe(MessageType::INVENTORY);
        entity -> subscribe(MessageType::ITEM_THROW);
        entity -> subscribe(MessageType::INTERACTION);
        entity -> subscribe(MessageType::GIVE);
        entity -> subscribe(MessageType::KILL_PLAYER);
        entity -> subscribe(MessageType::TOGGLE_GOD);
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
        GuiElement* gui = new Widget(pos,{624, 429},TextureManager::getTexture("inventory.png"));
        for (int i = 0; i < inventoryComponent -> size.X; i++) {
            gui -> addGuiElement(new ItemSlot({78+i*78,78},&(inventoryComponent -> containers[i][0])));
            for (int j = 1; j < inventoryComponent -> size.Y; j++)
                gui -> addGuiElement(new ItemSlot({78+i*78,117+j*78},&(inventoryComponent -> containers[i][j])));
        }
        GuiManager::manager -> addGuiElement(gui);
        return gui;
    }
    
    bool onMessage(const Message &message) override {
        if (message.type == MessageType::INVENTORY) {
            const InventoryMessage &msg = static_cast<const InventoryMessage&>(message);
            makeInventoryGui(msg.offset + Window::size/2);
            return true;
            
        } else if (message.type == MessageType::ITEM_THROW) {
            Item* item = inventoryComponent -> containers[selected][0].item;
            if (item != nullptr) {
                MessageManager::notify(SpawnItemMessage(item, positionComponent -> position + dirs2[directionComponent -> direction]));
                inventoryComponent -> containers[selected][0].item = nullptr;
            }
            return true;
            
        } else if (message.type == MessageType::INTERACTION) {
            const InteractionMessage &msg = static_cast<const InteractionMessage&>(message);
            Item* item = inventoryComponent -> containers[selected][0].item;
            if (item && item -> onClick(msg.attack)) return true;
            if (dist(msg.position, positionComponent -> position) > RADIUS) return false;
            MessageManager::notify(InteractionItemMessage(msg.position, msg.attack, item));
            return true;
            
        } else if (message.type == MessageType::GIVE) {
            const GiveMessage &msg = static_cast<const GiveMessage&>(message);
            inventoryComponent -> addItem(msg.item);
            return true;
        } else if (message.type == MessageType::KILL_PLAYER) {
            healthComponent -> health = 0;
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

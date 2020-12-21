//
//  ChestComponent.hpp
//  Game
//
//  Created by Valentin Imbach on 18.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "../Item.hpp"
#include "../Gui/GuiManager.hpp"
#include "Components.h"

#include "../MessagingSystem.hpp"

class ChestComponent : public Component {
public:
    static ComponentType componentType;
    InventoryComponent* inventoryComponent;
    SpriteComponent* spriteComponent;
    
    void init() override {
        inventoryComponent = entity -> getComponent<InventoryComponent>();
        spriteComponent = entity -> getComponent<SpriteComponent>();
        spriteComponent -> sprite = Sprite(TextureManager::getTexture("sprites.png"),{6,2},{1,2});
    }
    
    bool onMessage(const Message& message) override {
        if (message.type == MessageType::INTERACTION) {
            MessageManager::notify(InventoryMessage(Window::size/2 + pair<int>(0,100)));
            makeGui(Window::size/2 - pair<int>(0,180));
            return true;
        }
        return false;
    }
    
    GuiElement* makeGui(pair<int> pos) {
        GuiElement* gui = new Widget(pos,{306, 192},TextureManager::getTexture("chestGui.png"), true);
        for (int i = 0; i < inventoryComponent -> size.X; i++) {
            for (int j = 0; j < inventoryComponent -> size.Y; j++) {
                gui -> addGuiElement(new ItemSlot({39+i*57,39+j*57},&(inventoryComponent -> containers[i][j])));
            }
        }
        GuiManager::manager -> addGuiElement(gui);
        return gui;
    }
    
    Component* create() override {
        return new ChestComponent();
    }
    
};

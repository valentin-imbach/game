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
        spriteComponent -> height = 1;
        spriteComponent -> sprite = Sprite(TextureManager::getTexture("sprites.png"),{6,2},{1,2});
    }
    
    bool onMessage(const Message& message) override {
        if (message.type == MessageType::INTERACTION) {
            MessageManager::notify(InventoryMessage(pair<int>(0,150)));
            makeGui(pair<int>(0,-180));
            return true;
        }
        return false;
    }
    
    GuiElement* makeGui(pair<int> pos) {
        GuiElement* gui = new Widget(pos,{432, 192},TextureManager::getTexture("chestGui.png"), true);
        pair<int> size = inventoryComponent -> size;
        for (int i = size.X - 1; i >= 0; i--)
            for (int j = size.Y - 1; j >= 0; j--)
                gui -> addGuiElement(new ItemSlot({i*78-(size.X-1)*39,j*78-(size.Y-1)*39},&(inventoryComponent -> containers[i][j])));
        GuiManager::manager -> addGuiElement(gui);
        return gui;
    }
    
    Component* create() override {
        return new ChestComponent();
    }
    
};

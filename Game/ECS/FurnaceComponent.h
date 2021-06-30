//
//  FurnaceComponent.h
//  Game
//
//  Created by Valentin Imbach on 22.06.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "../Item.hpp"
#include "../Gui/GuiManager.hpp"

class FurnaceComponent : public Component {
public:
    static ComponentType componentType;
    SpriteComponent* spriteComponent;
    
    ItemContainer fuel = ItemContainer(ItemType::FUEL);
    ItemContainer input, output;
    
    void init() override {
        spriteComponent = entity -> getComponent<SpriteComponent>();
        spriteComponent -> height = 1;
        spriteComponent -> sprite = Sprite(TextureManager::getTexture("sprites.png"),{7,9},{1,2});
    }
    
    bool onMessage(const Message& message) override {
        if (message.type == MessageType::INTERACTION) {
            MessageManager::notify(InventoryMessage(pair<int>(0,150)));
            makeGui(Window::size/2 - pair<int>(0,160));
            return true;
        }
        return false;
    }
    
    GuiElement* makeGui(pair<int> pos) {
        GuiElement* gui = new Widget(pos, {432, 192}, TextureManager::getTexture("chestGui.png"), true);
        gui -> addGuiElement(new ItemSlot({2*48,48+5}, &input));
        gui -> addGuiElement(new ItemSlot({2*48,3*48-5}, &fuel));
        gui -> addGuiElement(new ItemSlot({6*48,2*48}, &output));
        GuiManager::manager -> addGuiElement(gui);
        return gui;
    }
    
    Component* create() override {
        return new TableComponent();
    }
    
};

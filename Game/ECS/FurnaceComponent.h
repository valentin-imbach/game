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
    
    ItemContainer a, b, c;
    
    void init() override {
        spriteComponent = entity -> getComponent<SpriteComponent>();
        spriteComponent -> height = 1;
        spriteComponent -> sprite = Sprite(TextureManager::getTexture("sprites.png"),{7,9},{1,2});
    }
    
    bool onMessage(const Message& message) override {
        if (message.type == MessageType::INTERACTION) {
            MessageManager::notify(InventoryMessage(Window::size/2 + pair<int>(0,100)));
            makeGui(Window::size/2 - pair<int>(0,200));
            return true;
        }
        return false;
    }
    
    GuiElement* makeGui(pair<int> pos) {
        GuiElement* gui = new Widget(pos, {270, 96}, TextureManager::getTexture("tableGui.png"), true);
        gui -> addGuiElement(new ItemSlot({48,48}, &a));
        gui -> addGuiElement(new ItemSlot({48+27*3,48}, &b));
        gui -> addGuiElement(new ItemSlot({48+57*3,48}, &c));
        GuiManager::manager -> addGuiElement(gui);
        return gui;
    }
    
    Component* create() override {
        return new TableComponent();
    }
    
};

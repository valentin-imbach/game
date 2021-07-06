//
//  TableComponent.h
//  Game
//
//  Created by Valentin Imbach on 28.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "../Item.hpp"
#include "../Gui/GuiManager.hpp"

class TableComponent : public Component {
public:
    static ComponentType componentType;
    SpriteComponent* spriteComponent;
    
    ItemContainer input, output;
    CraftingIndex craftingIndex;
    
    void init() override {
        spriteComponent = entity -> getComponent<SpriteComponent>();
        spriteComponent -> height = 1;
        spriteComponent -> sprite = Sprite(TextureManager::getTexture("sprites.png"),{5,9},{1,2});
        
        craftingIndex.add(ItemID::PINE_LOG, {ItemID::PINE_PLANK, 4});
        craftingIndex.add(ItemID::OAK_LOG, {ItemID::OAK_PLANK, 4});
        craftingIndex.add(ItemID::ROSEWOOD_LOG, {ItemID::ROSEWOOD_PLANK, 4});
        craftingIndex.add(ItemID::SPRUCE_LOG, {ItemID::SPRUCE_PLANK, 4});
        craftingIndex.add(ItemID::ASHWOOD_LOG, {ItemID::ASHWOOD_PLANK, 4});
        
        craftingIndex.add(ItemID::PINE_PLANK, {ItemID::PINE_STICK, 6});
        craftingIndex.add(ItemID::OAK_PLANK, {ItemID::OAK_STICK, 6});
        craftingIndex.add(ItemID::ROSEWOOD_PLANK, {ItemID::ROSEWOOD_STICK, 6});
        craftingIndex.add(ItemID::SPRUCE_PLANK, {ItemID::SPRUCE_STICK, 6});
        craftingIndex.add(ItemID::ASHWOOD_PLANK, {ItemID::ASHWOOD_STICK, 6});
    }
    
    bool onMessage(const Message& message) override {
        if (message.type == MessageType::INTERACTION) {
            MessageManager::notify(InventoryMessage(pair<int>(0,170)));
            makeGui(pair<int>(0,-150));
            return true;
        }
        return false;
    }
    
    void hammer() {
        craftingIndex.craft(&input, &output);
    }
    
    GuiElement* makeGui(pair<int> pos) {
        TabWidget* gui = new TabWidget(pos, {426, 192}, TextureManager::getTexture("tab.png"), true);
        Widget* tab1 = gui -> addTab(new Widget(pos, {426, 192}, TextureManager::getTexture("crafting.png"), true));
        tab1 -> addGuiElement(new ItemSlot({78,0}, &output));
        tab1 -> addGuiElement(new Button({0,0}, {54,42}, &TableComponent::hammer, this, TextureManager::getTexture("hammerButton.png"), TextureManager::getTexture("hammerButton2.png")));
        tab1 -> addGuiElement(new ItemSlot({-78,0}, &input));
        gui -> addTab(new Widget(pos, {426, 192}, TextureManager::getTexture("crafting.png"), true));
        gui -> addTab(new Widget(pos, {426, 192}, TextureManager::getTexture("crafting.png"), true));
        GuiManager::manager -> addGuiElement(gui);
        return gui;
    }
    
    Component* create() override {
        return new TableComponent();
    }
    
};

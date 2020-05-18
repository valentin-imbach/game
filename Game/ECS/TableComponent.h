//
//  TableComponent.h
//  Game
//
//  Created by Valentin Imbach on 28.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "../Item.h"
#include "../Gui/GuiManager.hpp"

class TableComponent : public Component {
public:
    ItemContainer a;
    ItemContainer b;
    ItemContainer c;
    
    TableComponent(Entity* entity, pair<int> pos) {
        entity -> addComponent<GridComponent>(pos);
        entity -> addComponent<SpriteComponent>("table.png");
    }
    
    bool handleEvent(SDL_Event event) override {
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_RIGHT) {
                GuiElement* inv = entity -> manager -> player -> getComponent<PlayerGuiComponent>() -> makeInventoryGui(Window::size/2 + pair<int>(0,100));
                makeGui(Window::size/2 - pair<int>(0,200), inv);
                return true;
            }
        }
        return false;
    }
    
    GuiElement* makeGui(pair<int> pos, GuiElement* link) {
        GuiElement* gui = new Widget(pos,{270, 96},TextureManager::loadTexture("tableGui.png"), link);
        gui -> addGuiElement(new ItemSlot({48,48},&a));
        gui -> addGuiElement(new ItemSlot({48+27*3,48},&b));
        gui -> addGuiElement(new ItemSlot({48+57*3,48},&c));
        GuiManager::manager -> addGuiElement(gui);
        return gui;
    }
    
};

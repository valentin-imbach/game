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
    }
    
    GuiElement* makeGui(pair<int> pos, GuiElement* link) {
        GuiElement* gui = new Widget(pos,{270, 96},TextureManager::tableTexture, link);
        gui -> addGuiElement(new ItemSlot({48,48},&a));
        gui -> addGuiElement(new ItemSlot({48+27*3,48},&b));
        gui -> addGuiElement(new ItemSlot({48+57*3,48},&c));
        GuiManager::manager -> addGuiElement(gui);
        return gui;
    }
    
};

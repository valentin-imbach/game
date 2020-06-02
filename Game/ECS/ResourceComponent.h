//
//  ResourceComponent.h
//  Game
//
//  Created by Valentin Imbach on 28.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "../Item.h"

class ResourceComponent : public Component {
public:
    static ComponentType componentType;
    int type;
    LootTable loot;

    ResourceComponent(int t = 0) {
        type = t;
        loot.addLoot(6,2,5);
        loot.addLoot(7,2,5);
    }
    
    bool handleEvent(SDL_Event event) override {
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                if (entity -> manager -> player -> getComponent<PlayerGuiComponent>() -> getSelectedItem() -> type() == type) {
                    entity -> active = false;
                    for (Loot l : loot.table) {
                        Entity* item = entity -> manager -> addEntity();
                        item -> addComponent<PositionComponent>(entity -> getComponent<PositionComponent>() -> position);
                        item -> addComponent<CollisionComponent>(0.25,0.25,0.25,0.25);
                        item -> addComponent<ItemComponent>(l.createItem());
                    }
                    return true;
                }
            }
        }
        return false;
    }
    
};

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
    int type;
    LootTable loot;

    ResourceComponent() {}
    ResourceComponent(Entity* entity, pair<int> pos = {0,0}, int t = 0, const char* path = "") {
        entity -> addComponent<GridComponent>(pos);
        entity -> addComponent<SpriteComponent>(path);
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
                        item -> addComponent<ItemComponent>(entity -> getComponent<PositionComponent>() -> position, l.createItem());
                    }
                    return true;
                }
            }
        }
        return false;
    }
    
};

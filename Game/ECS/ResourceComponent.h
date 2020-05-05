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

    ResourceComponent(Entity* entity, pair<int> pos, int t) {
        entity -> addComponent<GridComponent>(pos);
        type = t;
        loot.addLoot(6,2,5);
        loot.addLoot(7,2,5);
    }
    bool mine(Item* tool) {
        if (tool -> type == type) {
            entity -> active = false;
            for (Loot l : loot.table) {
                Entity* item = entity -> manager -> addEntity();
                item -> addComponent<ItemComponent>(entity -> getComponent<PositionComponent>() -> position, l.createItem());
            }
            return true;
        }
        return false;
    }
};

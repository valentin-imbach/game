//
//  EntityFactory.cpp
//  Game
//
//  Created by Valentin Imbach on 11.06.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "EntityFactory.hpp"

void EntityFactory::createEntity(EntityManager* manager, int type, pair<int> pos) {
    if (type < 100) {
        Entity* res = manager -> addEntity();
        res -> addComponent<PositionComponent>(pos);
        res -> addComponent<SpriteComponent>();
        res -> addComponent<GridComponent>();
        res -> addComponent<ResourceComponent>(type);
    } else if (type == (int)EntityType::TABLE) {
        Entity* table = manager -> addEntity();
        table -> addComponent<PositionComponent>(pos);
        table -> addComponent<SpriteComponent>();
        table -> addComponent<GridComponent>();
        table -> addComponent<TableComponent>();
    } else if (type == (int)EntityType::CHEST) {
        Entity* chest = manager -> addEntity();
        chest -> addComponent<InventoryComponent>(5,3);
        chest -> addComponent<PositionComponent>(pos);
        chest -> addComponent<SpriteComponent>();
        chest -> addComponent<GridComponent>();
        chest -> addComponent<ChestComponent>();
    }
}

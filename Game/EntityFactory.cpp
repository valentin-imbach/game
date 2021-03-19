//
//  EntityFactory.cpp
//  Game
//
//  Created by Valentin Imbach on 11.06.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "EntityFactory.hpp"

void EntityFactory::createEntity(EntityManager* manager, int type, pair<int> pos) {
    if (!manager -> isFree(pos.X, pos.Y)) return;
    if (type < (int)RESOURCE::MAX) {
        Entity* res = manager -> addEntity();
        res -> addComponent<PositionComponent>(pos);
        res -> addComponent<SizeComponent>();
        res -> addComponent<SpriteComponent>();
        res -> addComponent<GridComponent>(pos);
        res -> addComponent<ResourceComponent>(type);
    } else if (type >= 10 && type - 10 < (int)PICKUP::MAX) {
        Entity* branch = manager -> addEntity();
        branch -> addComponent<PositionComponent>(pos);
        branch -> addComponent<SizeComponent>();
        branch -> addComponent<SpriteComponent>();
        branch -> addComponent<GridComponent>(pos) -> solid = false;
        branch -> addComponent<PickupComponent>(type-10);
    } else if (type == (int)EntityType::TABLE) {
        Entity* table = manager -> addEntity();
        table -> addComponent<PositionComponent>(pos);
        table -> addComponent<SizeComponent>();
        table -> addComponent<SpriteComponent>();
        table -> addComponent<GridComponent>(pos);
        table -> addComponent<TableComponent>();
    } else if (type == (int)EntityType::CHEST) {
        Entity* chest = manager -> addEntity();
        chest -> addComponent<InventoryComponent>(5,2);
        chest -> addComponent<SizeComponent>();
        chest -> addComponent<PositionComponent>(pos);
        chest -> addComponent<SpriteComponent>();
        chest -> addComponent<GridComponent>(pos);
        chest -> addComponent<ChestComponent>();
    } else if (type == (int)EntityType::FURNACE) {
        Entity* furnace = manager -> addEntity();
        furnace -> addComponent<PositionComponent>(pos);
        furnace -> addComponent<SizeComponent>();
        furnace -> addComponent<SpriteComponent>();
        furnace -> addComponent<GridComponent>(pos);
        furnace -> addComponent<FurnaceComponent>();
    }
}

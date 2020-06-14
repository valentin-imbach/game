//
//  EntityFactory.cpp
//  Game
//
//  Created by Valentin Imbach on 11.06.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "EntityFactory.hpp"

void EntityFactory::createEntity(EntityManager* manager, EntityType type, pair<int> pos) {
    if (type == EntityType::ROCK) {
        Entity* rock = manager -> addEntity();
        rock -> addComponent<PositionComponent>(pos);
        rock -> addComponent<SpriteComponent>();
        rock -> addComponent<GridComponent>(pair<int>(1,1));
        rock -> addComponent<CollisionComponent>();
        rock -> addComponent<ResourceComponent>(1);
    } else if (type == EntityType::TABLE) {
        Entity* table = manager -> addEntity();
        table -> addComponent<PositionComponent>(pos);
        table -> addComponent<SpriteComponent>();
        table -> addComponent<GridComponent>();
        table -> addComponent<CollisionComponent>();
        table -> addComponent<TableComponent>();
    } else if (type == EntityType::CHEST) {
        Entity* chest = manager -> addEntity();
        chest -> addComponent<InventoryComponent>(5,3);
        chest -> addComponent<PositionComponent>(pos);
        chest -> addComponent<SpriteComponent>();
        chest -> addComponent<GridComponent>();
        chest -> addComponent<CollisionComponent>();
        chest -> addComponent<ChestComponent>();
    }
}

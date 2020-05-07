//
//  ItemComponent.h
//  Game
//
//  Created by Valentin Imbach on 29.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "Components.h"
#include "../Item.h"

class ItemComponent : public Component {
private:
    pair<float> position;
    PositionComponent *positionComponent;
    CollisionComponent *collisionComponent;
public:
    Item* item;
    ItemComponent(Entity* entity, pair<float> pos, Item* item) {
        position = pos;
        position.X += (RAND - 0.5)/2;
        position.Y += (RAND - 0.5)/2;
        this -> item = item;
        positionComponent = entity -> addComponent<PositionComponent>(position);
        collisionComponent = entity -> addComponent<CollisionComponent>(0.25,0.25,0.25,0.25);
        entity -> addTag(TAG::ITEM);
    }
    
    void render() override {
        pair<float> position = Camera::gtos(positionComponent -> position);
        item -> render(position.X,position.Y,32,false);
    }
    
};

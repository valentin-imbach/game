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
    ItemComponent(pair<float> pos, Item* item) {
        position = pos;
        this -> item = item;
    }
    
    void init() override {
        positionComponent = &entity -> addComponent<PositionComponent>(position);
        collisionComponent = &entity -> addComponent<CollisionComponent>(0.25,0.25,0.25,0.25);
    }
    
    void render() override {
        pair<float> position = Camera::gtos(positionComponent -> position);
        item -> render(position.X-16,position.Y-16,32);
    }
    
};

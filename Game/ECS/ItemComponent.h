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
    float x, y;
    Item* item;
    PositionComponent *positionComponent;
    CollisionComponent *collisionComponent;
public:
    ItemComponent(float x, float y, Item* item) {
        this -> x = x;
        this -> y = y;
        this -> item = item;
    }
    
    void init() override {
        positionComponent = &entity -> addComponent<PositionComponent>(x,y);
        collisionComponent = &entity -> addComponent<CollisionComponent>(0.25,0.25,0.25,0.25);
    }
    
    void render() override {
        pair<float> pos = Camera::gtos({x,y});
        item -> render(pos.X-16,pos.Y-16,32);
    }
    
};

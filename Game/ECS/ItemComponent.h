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
    Item* item;
    pair<float> pos;
    PositionComponent *positionComponent;
    CollisionComponent *collisionComponent;
public:
    ItemComponent(float x, float y, Item* item) {
        pos = {x,y};
        this -> item = item;
    }
    
    void init() override {
        positionComponent = &entity -> addComponent<PositionComponent>(pos.X,pos.Y);
        collisionComponent = &entity -> addComponent<CollisionComponent>(0.25,0.25,0.25,0.25);
    }
    
    void render() override {
        pair<float> pos = Camera::gtos(positionComponent -> pos);
        item -> render(pos.X-16,pos.Y-16,32);
    }
    
};

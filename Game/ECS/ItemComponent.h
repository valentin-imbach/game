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
    PositionComponent *positionComponent;
    CollisionComponent *collisionComponent;
public:
    static ComponentType componentType;
    Item* item;

    ItemComponent(Item* item = nullptr) {
        this -> item = item;
    }
    
    void init() {
        positionComponent = entity -> getComponent<PositionComponent>();
        collisionComponent = entity -> getComponent<CollisionComponent>();
        entity -> addTag(TAG::ITEM);
    }
    
    void render() override {
        pair<float> position = Camera::gtos(positionComponent -> position);
        item -> render(position.X,position.Y,Camera::ZOOM/2,false);
    }
    
};

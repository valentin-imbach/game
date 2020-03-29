//
//  CollisionComponent.h
//  Game
//
//  Created by Valentin Imbach on 28.03.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

#include "../CollisionManager.h"
#include "../Camera.hpp"

class CollisionComponent : public Component {
public:
    
    Collider collider;
    PositionComponent *positionComponent;
    bool visible = true;
    
    float offsetX;
    float offsetY;
    
    CollisionComponent() {}
    
    CollisionComponent(float e, float n, float w, float s) {
        collider.w = e+w;
        collider.h = n+s;
        offsetX = w;
        offsetY = n;
    }
    
    void init() override {
        positionComponent = &entity -> addComponent<PositionComponent>();
    }
    
    void update() override {
        collider.x = positionComponent -> x - offsetX;
        collider.y = positionComponent -> y - offsetY;
    }
    
    void render() override {
        if (visible) {
            Camera::renderRect(collider.x,collider.y,collider.w,collider.h);
        }
    }
    
};

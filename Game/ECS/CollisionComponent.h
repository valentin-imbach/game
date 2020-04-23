//
//  CollisionComponent.h
//  Game
//
//  Created by Valentin Imbach on 28.03.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "Components.h"
#include "../CollisionManager.h"
#include "../Camera.hpp"

class CollisionComponent : public Component {
public:
    
    Collider collider;
    PositionComponent *positionComponent;
    
    float offsetX;
    float offsetY;
    
    CollisionComponent() {
        offsetX = 0.5;
        offsetY = 0.5;
    }
    
    CollisionComponent(float e, float n, float w, float s) {
        collider.w = e+w;
        collider.h = n+s;
        offsetX = w;
        offsetY = n;
    }
    
    void init() override {
        positionComponent = &entity -> getComponent<PositionComponent>();
    }
    
    void update() override {
        collider.x = positionComponent -> pos.X - offsetX;
        collider.y = positionComponent -> pos.Y - offsetY;
    }
    
    void debugRender() override {
        Camera::renderRect(collider.x,collider.y,collider.w,collider.h);
    }
    
};

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
    
    pair<float> offset;
    
    CollisionComponent(float e = 0.5, float n = 0.5, float w = 0.5, float s = 0.5) {
        collider.size = {e+w,n+s};
        offset = {w,n};
    }
    
    void init() override {
        positionComponent = &entity -> getComponent<PositionComponent>();
        update();
    }
    
    void update() override {
        collider.position = positionComponent -> position - offset;
    }
    
    void debugRender() override {
        Camera::drawRect(collider.position,collider.size);
    }
    
};

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
    
    CollisionComponent(float w, float h) {
        collider.w = w;
        collider.h = h;
    }
    
    void init() override {
        assert(entity -> hasComponent<PositionComponent>());
        positionComponent = &entity -> getComponent<PositionComponent>();
    }
    
    void update() override {
        collider.x = positionComponent -> x;
        collider.y = positionComponent -> y;
    }
    
    void render() override {
        if (visible) {
            Camera::renderRect(collider.x,collider.y,collider.w,collider.h);
        }
    }
    
};

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
    static ComponentType componentType;
    Collider collider;
    pair<float> offset;
    PositionComponent *positionComponent;
    
    CollisionComponent(float e = 0.5, float n = 0.5, float w = 0.5, float s = 0.5) : offset(w,n) {
        collider.size = {e+w,n+s};
    }
    
    void init() override {
        positionComponent = entity -> getComponent<PositionComponent>();
    }
    
    void update() override {
        collider.position = positionComponent -> position - offset;
    }
    
    void debugRender() override {
        Camera::drawRect(collider.position,collider.size);
    }
    
    SERIALIZE(collider,offset);
    
    Component* create() override {
        return new CollisionComponent();
    }
    
};

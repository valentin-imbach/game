//
//  GridComponent.h
//  Game
//
//  Created by Valentin Imbach on 06.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

#include "Components.h"

class GridComponent : public Component {
public:
    static ComponentType componentType;
    PositionComponent *positionComponent;
    
    pair<int> size;
    pair<int> anchor;
    
    bool solid = true;
    
    GridComponent(pair<int> a = {0,0}, pair<int> s = {1,1}) : anchor(a), size(s) {}
    
    void init() override {
        positionComponent = entity -> getComponent<PositionComponent>();
        entity -> addTag(EntityTag::STRUCT);
        MessageManager::notify(PlaceMessage(entity));
    }
    
    ~GridComponent() {
        MessageManager::notify(BreakMessage(entity));
    }
    
    Component* create() override {
        return new GridComponent();
    }
    
};

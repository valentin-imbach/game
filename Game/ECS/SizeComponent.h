//
//  SizeComponent.h
//  Game
//
//  Created by Valentin Imbach on 26/01/2021.
//  Copyright © 2021 Valentin Imbach. All rights reserved.
//

#pragma once
#include "Components.h"

class SizeComponent : public Component {
public:
    static ComponentType componentType;
    pair<float> size;

    SizeComponent(pair<float> s = {1,1}) : size(s) {}
    
    SERIALIZE(size);
    
    Component* create() override {
        return new SizeComponent();
    }
    
};

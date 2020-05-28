//
//  PositionComponent.h
//  Game
//
//  Created by Valentin Imbach on 26.03.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "Components.h"

class PositionComponent : public Component {
public:
    pair<float> position;
    PositionComponent(Entity* entity, pair<float> pos) {
        position = pos;
    }
    
    void serialize(std::fstream& stream) override {
        stream.write((char*)&position,sizeof(position));
    }
    
    void deserialize(std::fstream& stream) override {
        stream.read((char*)&position,sizeof(position));
    }
    
};


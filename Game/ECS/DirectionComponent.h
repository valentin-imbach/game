//
//  DirectionComponent.h
//  Game
//
//  Created by Valentin Imbach on 27.03.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "Components.h"

class DirectionComponent : public Component {
public:
    Direction direction;
    DirectionComponent(Entity* entity, Direction dir = SOUTH) {
        direction = dir;
    }
    
    void serialize(std::fstream& stream) override {
        stream.write((char*)&direction,sizeof(direction));
    }
    
    void deserialize(std::fstream& stream) override {
        stream.read((char*)&direction,sizeof(direction));
    }
};

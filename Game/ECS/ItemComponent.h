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
    pair<float> position;
    PositionComponent *positionComponent;
    CollisionComponent *collisionComponent;
public:
    Item* item;
    ItemComponent() {}
    ItemComponent(Entity* entity, pair<float> pos = {0,0}, Item* item = nullptr) {
        position = pos;
        position.X += (RAND - 0.5)/2;
        position.Y += (RAND - 0.5)/2;
        this -> item = item;
        positionComponent = entity -> addComponent<PositionComponent>(position);
        collisionComponent = entity -> addComponent<CollisionComponent>(0.25,0.25,0.25,0.25);
        entity -> addTag(TAG::ITEM);
    }
    
    void render() override {
        pair<float> position = Camera::gtos(positionComponent -> position);
        item -> render(position.X,position.Y,Camera::ZOOM/2,false);
    }
    
    void serialize(std::fstream& stream) override {
        //item -> serialize(stream)
        stream.write((char*)&position,sizeof(position));
    }
    
    void deserialize(std::fstream& stream) override {
        //item = new Itme();
        //item -> desirealize(stream);
        stream.read((char*)&position,sizeof(position));
    }
    
};

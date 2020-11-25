//
//  ItemComponent.h
//  Game
//
//  Created by Valentin Imbach on 29.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "Components.h"
#include "../Item.hpp"

class ItemComponent : public Component {
private:
    PositionComponent *positionComponent;
    CollisionComponent *collisionComponent;
public:
    static ComponentType componentType;
    Item* item;

    ItemComponent(Item* item = nullptr) {
        this -> item = item;
    }
    
    void init() override {
        positionComponent = entity -> getComponent<PositionComponent>();
        collisionComponent = entity -> getComponent<CollisionComponent>();
        collisionComponent -> collider.size = {0.5,0.5};
        collisionComponent -> offset = {0.25,0.25};
        entity -> addTag(EntityTag::ITEM);
    }
    
    void render() override {
        pair<float> position = Camera::gtos(positionComponent -> position);
        if (item == nullptr) {
            TextureManager::drawTexture(TextureManager::getTexture("cross.png"), position.X, position.Y, Camera::ZOOM/2, Camera::ZOOM/2,true);
        } else {
            item -> render(position.X,position.Y,Camera::ZOOM/2,false);
        }
    }
    
    Component* create() override {
        return new ItemComponent();
    }
    
};

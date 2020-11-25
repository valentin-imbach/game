//
//  SpriteComponent.h
//  Game
//
//  Created by Valentin Imbach on 27.03.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "Components.h"
#include "TextureManager.hpp"
#include "Camera.hpp"

class SpriteComponent : public Component {
public:
    static ComponentType componentType;
    PositionComponent* positionComponent;
    Sprite sprite;

    SpriteComponent(const char* path = "", pair<int> pos = {0,0}, pair<int> size = {1,1}) {
        sprite = Sprite(TextureManager::getTexture(path),pos,size);
        if (!sprite.texture) { sprite.texture = TextureManager::getTexture("default.png"); }
    }
    
    void init() override {
        positionComponent = entity -> getComponent<PositionComponent>();
    }

    void render() override {
        Camera::drawSprite(sprite, positionComponent -> position);
    }
    
    Component* create() override {
        return new SpriteComponent();
    }
    
};

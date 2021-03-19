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
    SizeComponent* sizeComponent;
    Sprite sprite;
    
    int height;

    SpriteComponent(Sprite s = Sprite(), int h = 0) : sprite(s), height(h) {
        if (!sprite.texture) sprite.texture = TextureManager::getTexture("default.png");
    }
    
    void init() override {
        positionComponent = entity -> getComponent<PositionComponent>();
        sizeComponent = entity -> getComponent<SizeComponent>();
    }

    void render() override {
        float x = (positionComponent -> position).X - (sizeComponent -> size).X/2;
        float y = (positionComponent -> position).Y - (sizeComponent -> size).Y/2 - height;
        Camera::drawSprite(sprite, {x,y});
    }
    
    Component* create() override {
        return new SpriteComponent();
    }
};

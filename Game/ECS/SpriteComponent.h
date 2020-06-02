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
    SDL_Texture* texture = nullptr;
    int sx, sy, sw, sh;
    pair<float> size;
    float offset = 0;
    Sprite sprite;

    SpriteComponent(const char* path = "", int x = 0, int y = 0, int w = 1, int h = 1) {
        size = pair<float>(w,h);
        sx = x;
        sy = y;
        sw = w;
        sh = h;
        sprite = Sprite(TextureManager::getTexture(path),{x,y},{w,h});
    }
    
    void init() override {
        positionComponent = entity -> getComponent<PositionComponent>();
    }

    void render() override {
        if (!sprite.texture) { sprite.texture = TextureManager::getTexture("default.png"); }
        Camera::drawSprite(sprite, positionComponent -> position);
    }
    
    ~SpriteComponent() override {
        SDL_DestroyTexture(texture);
    }
    
    Component* create() override {
        return new SpriteComponent();
    }
    
};

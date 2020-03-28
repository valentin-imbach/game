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
    PositionComponent *positionComponent;
    SDL_Texture *texture;
    SDL_Rect src;
    int offset = 0;

    SpriteComponent() {}
    SpriteComponent(const char* path) {
        texture = TextureManager::loadTexture(path);
        src.x = src.y = 0;
        src.w = src.h = 16;
    }
    
    void init() override {
        assert(entity -> hasComponent<PositionComponent>());
        positionComponent = &entity -> getComponent<PositionComponent>();
    }

    void render() override {
        Camera::renderTexture(texture, src, positionComponent -> x, positionComponent -> y - offset);
    }
    
    ~SpriteComponent() override {
        SDL_DestroyTexture(texture);
    }
};

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
    SDL_Texture *texture = nullptr;
    int sx, sy, sw, sh;
    pair<float> size;
    float offset = 0;

    SpriteComponent(Entity* entity, const char* path, int x = 0, int y = 0, int w = 1, int h = 1) {
        positionComponent = entity -> getComponent<PositionComponent>();
        texture = TextureManager::loadTexture(path);
        if (!texture) { texture = TextureManager::loadTexture("default.png"); }
        size = pair<float>(w,h);
        sx = x;
        sy = y;
        sw = w;
        sh = h;
    }

    void render() override {
        Camera::drawTexture(texture, sx, sy, sw, sh, positionComponent -> position, size, offset, true);
    }
    
    ~SpriteComponent() override {
        SDL_DestroyTexture(texture);
    }
};

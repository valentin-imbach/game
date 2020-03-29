//
//  Camera.hpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "tools.h"
#include "TextureManager.hpp"
#include "Window.hpp"

#define ZOOM 64
#define RADIUS 0.1f

class Camera {
public:
    inline static void setPos(pint p) { pos = p; }
    static void placeTexture(SDL_Texture* tex, int x, int y, int w, int h, pout pos);
    static void renderTexture(SDL_Texture* tex, SDL_Rect src, float x, float y, float offset);
    static void renderRect(float x, float y, float w, float h);
    static void setTarget(pint* t);
    static pint getPosition(pint);
    static void update();
    static pint pos;
    static int mode;
    static int speed;
    static pint target;
    static void handleEvents();
};

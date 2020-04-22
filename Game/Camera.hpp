//
//  Camera.hpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#pragma once

#include "tools.h"
#include "TextureManager.hpp"
#include "Window.hpp"

#define ZOOM 64
#define RADIUS 0.1f

class Camera {
public:
    static void placeTexture(SDL_Texture* tex, int x, int y, int w, int h, pair<float> pos);
    static void renderTexture(SDL_Texture* tex, SDL_Rect src, float x, float y, float offset);
    static void renderRect(float x, float y, float w, float h);
    static void update(pair<float> p);
    static void render();
    static pair<float> pos;
    static int mode;
    static int speed;
    static void handleEvents();
    static pair<float> stog(pair<float> p);
    static pair<float> gtos(pair<float> p);
};

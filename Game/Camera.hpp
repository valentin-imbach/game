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

class Camera {
public:
    static void drawTexture(SDL_Texture* tex, int sx, int sy, int sw, int sh, pair<float> dest, pair<int> size, int offset = 0, bool centred = false, int alpha = 255);
    static void drawRect(pair<float> pos, pair<float> size);
    static void drawSprite(Sprite sprite, pair<float> pos, bool centred = false, int alpha = 255);
    static void update(pair<float> p);
    static void render();
    static int ZOOM;
    static pair<float> position;
    static void handleEvents();
    static pair<float> stog(pair<int> p);
    static pair<int> gtos(pair<float> p);
};

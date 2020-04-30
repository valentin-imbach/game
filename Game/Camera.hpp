//
//  Camera.hpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#pragma once

#include "tools.h"

#define ZOOM 64
#define RADIUS 0.1f

class Camera {
public:
    static void drawTexture(SDL_Texture* tex, int sx, int sy, int sw, int sh, pair<float> dest, pair<int> size, int offset = 0, bool contre = false);
    static void drawRect(pair<float> pos, pair<float> size);
    static void update(pair<float> p);
    static void render();
    static pair<float> position;
    static void handleEvents();
    static pair<float> stog(pair<float> p);
    static pair<float> gtos(pair<float> p);
};

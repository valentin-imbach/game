//
//  TextManager.hpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#pragma once

#include "Window.hpp"
#include <SDL2_ttf/SDL_ttf.h>

class TextManager {
public:
    static void Init();
    static void drawText(const char* text, int x, int y);
    static TTF_Font* font;
    static SDL_Color color;
    static void loadFont(const char* path, int size);
    inline static void setColor(Uint8 r, Uint8 g, Uint8 b) { color = {r,g,b}; }
};


//
//  TextManager.hpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#pragma once
#include "tools.h"
#include <SDL2_ttf/SDL_ttf.h>

class TextManager {
public:
    static TTF_Font* font;
    static SDL_Color color;
    
    static void Init();
    static void drawText(std::string text, pair<int> pos, bool centre = false, SDL_Color color = {255,255,255});
    static void drawText(int a, pair<int> pos, bool centre = false, SDL_Color color = {255,255,255});
    static void loadFont(const char* path, int size);
};


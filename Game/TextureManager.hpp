//
//  TextureManager.hpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#pragma once
#include "tools.h"
#include "Window.hpp"

#define BIT 16

class TextureManager {
public:
    static v(SDL_Texture*) tilesets;
    static SDL_Texture* icons;
    static SDL_Texture* icons2;
    static std::map<std::string, SDL_Texture*> textures;
    
    static void Init();
    static void refresh();
    static SDL_Texture* getTexture(std::string path);
    static void drawTexture(SDL_Texture* tex, int dx, int dy, bool centered = false);
    static void drawTexture(SDL_Texture* tex, int dx, int dy, int dw, int dh, bool centered = false);
    static void drawTexture(SDL_Texture* tex, int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh, bool centered = false, int alpha = 255);
    static void drawRect(pair<int> pos, pair<int> size, int r = 0, int g = 0, int b = 0);
private:
    static SDL_Texture* loadTexture(std::string path);
    static void loadTilesets();
    static void loadIcons();
};

class Sprite {
public:
    SDL_Texture* texture;
    pair<int> size;
    int priority = 0;
    pair<int> anker;
    bool running = true;
    
    Sprite(SDL_Texture* tex = nullptr, pair<int> a = {0,0}, pair<int> s = {1,1}, int l = 1, pair<int> o = {1,0}, int d = 20) : texture(tex), anker(a), size(s), length(l), offset(o), delay(d) {}
    
    pair<int> getPosition() {
        if (!running) return anker;
        int index = ((Window::ticks-resetTime)/delay) % length;
        return {anker.X + index * offset.X, anker.Y + index * offset.Y};
    }
    
    void reset() {
        resetTime = Window::ticks;
    }
    
private:
    pair<int> offset;
    int resetTime = 0;
    int length;
    int delay;
};

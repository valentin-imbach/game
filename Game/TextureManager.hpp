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

struct AnimatedSprite {
    SDL_Texture* texture;
    pair<int> size;
    v(pair<int>) positions;
    int priority;
    int length;
    int speed;
    
    AnimatedSprite(SDL_Texture* tex, v(pair<int>) pos, pair<int> s = {1,1}, int p = 0) : texture(tex), positions(pos), size(s), priority(p) {
        length = (int)positions.size();
        speed = 100;
    }
    virtual pair<int> getPosition() {
        int index = (Window::ticks/speed) % length;
        return positions[index];
    }
};

struct Sprite : public AnimatedSprite {
    pair<int> position;
    Sprite(SDL_Texture* tex = nullptr, pair<int> pos = {0,0}, pair<int> s = {1,1}, int p = 0) : AnimatedSprite(tex,{pos},s,p), position(pos) {}
    pair<int> getPosition() override {
        return position;
    }
};


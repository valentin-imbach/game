//
//  GuiElements.hpp
//  Game
//
//  Created by Valentin Imbach on 10.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "tools.h"

class GuiElement {
protected:
    pair<int> position;
    pair<int> size;
    SDL_Texture* texture;
    
    GuiElement* parent = nullptr;
    v(GuiElement*) children;
    
public:
    virtual void render();
    virtual bool handleEvent(SDL_Event event);
};

class Widget : public GuiElement {
public:
    Widget(pair<int> pos, pair<int> s, SDL_Texture* tex);
};

class Button : public GuiElement {
public:
    Button(pair<int> pos, pair<int> s, SDL_Texture* tex = nullptr);
};

class ItemSlot : public GuiElement {
public:
    ItemSlot(pair<int> pos, pair<int> s, SDL_Texture* tex);
    void render() override;
};



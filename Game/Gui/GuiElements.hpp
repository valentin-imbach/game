//
//  GuiElements.hpp
//  Game
//
//  Created by Valentin Imbach on 10.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "tools.h"
#include "../Item.h"

class GuiManager2;

class GuiElement {
protected:
    pair<int> position;
    pair<int> size;
    SDL_Texture* texture;
    GuiElement* parent = nullptr;
    v(GuiElement*) children;
    bool check(pair<int> p);
    
    friend GuiManager2;

public:
    bool alive = true;
    GuiManager2* manager = nullptr;
    
    virtual bool onClick(int b) { return false; }
    virtual bool onKey(int k) { return false; }
    
    GuiElement(pair<int> pos, pair<int> s, SDL_Texture* tex = nullptr);
    
    void addGuiElement(GuiElement* gui);
    void setParent(GuiElement* gui);
    void setManager(GuiManager2* m);
    bool handleEvent(SDL_Event event);
    void destroy();
    
    virtual void render();
    virtual void update();
    
};

class Widget : public GuiElement {
public:
    Widget(pair<int> pos, pair<int> s, SDL_Texture* tex = nullptr);
    bool onKey(int key) override;
};

class Button : public GuiElement {
public:
    Button(pair<int> pos, pair<int> s, SDL_Texture* tex = nullptr);
};

class ItemSlot : public GuiElement {
private:
    ItemContainer* itemContainer;
public:
    ItemSlot(pair<int> pos, ItemContainer* item);
    void render() override;
    bool onClick(int b) override;
};

//
//  GuiElements.hpp
//  Game
//
//  Created by Valentin Imbach on 10.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "tools.h"
#include "../Item.hpp"
#include "../Window.hpp"

enum class Alignment {
    NORTH_WEST,
    NORTH,
    NORTH_EAST,
    WEST,
    CENTER,
    EAST,
    SOUTH_WEST,
    SOUTH,
    SOUTH_EAST
};

class GuiManager;

class GuiElement {
protected:
    pair<int> position;
    Alignment align;
    pair<int> relative_pos;
    pair<int> size;
    SDL_Texture* texture;
    SDL_Texture* hoverTexture = nullptr;
    GuiElement* parent = nullptr;
    v(GuiElement*) children;
    bool check(pair<int> p);

public:
    bool alive = true;
    bool hover = false;
    bool absorbHover = false;
    bool absorbKeystate = false;
    
    virtual bool onClick(int b) { return false; }
    virtual bool onKey(int k) { return false; }
    virtual bool onScroll(int y) { return false; }
    virtual bool onText(std::string s) { return false; }
    
    GuiElement(pair<int> pos, pair<int> s = {0,0}, SDL_Texture* tex = nullptr, Alignment ali = Alignment::CENTER);
    
    void addGuiElement(GuiElement* gui);
    bool handleEvent(SDL_Event event);
    
    void render();
    virtual void extraRender() {}
    virtual void update();
};

class Widget : public GuiElement {
private:
    bool weak;
public:
    Widget(pair<int> pos, pair<int> s, SDL_Texture* tex = nullptr, bool w = false) : GuiElement(pos, s, tex), weak(w) {
        absorbKeystate = true;
    }
    bool onKey(int key) override;
};

class TextElement : public GuiElement {
private:
    std::string text;
    bool centre;
public:
    TextElement(pair<int> pos, std::string t, bool c = false);
    void extraRender() override;
};

class DisplayElement : public GuiElement {
private:
    int* value;
public:
    DisplayElement(pair<int> pos, int* v);
    void extraRender() override;
};

class Button : public GuiElement {
private:
    void(*function)();
public:
    Button(pair<int> pos, pair<int> s, void(*func)(), SDL_Texture* tex, SDL_Texture* tex2 = nullptr);
    bool onClick(int b) override;
};

class TextField : public GuiElement {
public:
    std::string text;
    bool active = false;
    
    TextField(pair<int> pos, pair<int> s, SDL_Texture* tex);
    bool onClick(int b) override;
    bool onKey(int k) override;
    bool onText(std::string t) override;
    void extraRender() override;
};

class ItemSlot : public GuiElement {
protected:
    ItemContainer* itemContainer;
public:
    ItemSlot(pair<int> pos, ItemContainer* c);
    void extraRender() override;
    bool onClick(int b) override;
};

class MouseSlot : public ItemSlot {
public:
    MouseSlot();
    void update() override;
    void extraRender() override;
};

class Hotbar : public GuiElement {
private:
    v(ItemContainer*) hotbarContainers;
    int* selected;
    
public:
    Hotbar(v(ItemContainer*) items, int* sel);
    void extraRender() override;
    bool onScroll(int y) override;
    bool onKey(int k) override;
    bool onClick(int b) override;
};


class HealthBar : public GuiElement {
public:
    int* health;
    SDL_Texture* heart;
    HealthBar(int* h);
    void extraRender() override;
};


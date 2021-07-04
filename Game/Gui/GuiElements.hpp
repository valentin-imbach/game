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
#include "../Debug.h"

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
    virtual bool preHandleEvent(SDL_Event event) { return false; }
    
    void reposition();
    virtual void render();
    virtual void preRender() {}
    virtual void postRender() {}
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

class TabWidget : public Widget {
private:
    v(GuiElement*) tabs;
public:
    int selected = 0;
    TabWidget(pair<int> pos, pair<int> s, SDL_Texture* tabTex, bool weak = false) : Widget(pos, s, nullptr, weak) {}
    Widget* addTab(Widget* tab);
    void update() override;
    void preRender() override;
    bool preHandleEvent(SDL_Event event) override;
};

class Tab : public GuiElement {
private:
    int number;
    TabWidget* context;
public:
    Tab(pair<int> pos, pair<int> size, int n, TabWidget* c);
    bool onClick(int b) override;
    void preRender() override;
};

class TextElement : public GuiElement {
private:
    std::string text;
    bool centre;
public:
    TextElement(pair<int> pos, std::string t, bool c = false, Alignment align = Alignment::CENTER);
    void preRender() override;
};

class DisplayElement : public GuiElement {
private:
    int* value;
public:
    DisplayElement(pair<int> pos, int* v);
    void preRender() override;
};

template <typename T>
class Button : public GuiElement {
private:
    void(T::*function)();
    T* context;
public:
    Button(pair<int> pos, pair<int> s, void(T::*func)(), T* c, SDL_Texture* tex, SDL_Texture* tex2 = nullptr) : GuiElement(pos, s, tex) {
        hoverTexture = tex2;
        function = func;
        context = c;
    }
    bool onClick(int b) override {
        if (check(Window::mousePos)) {
            if (b == SDL_BUTTON_LEFT) (context->*function)();
            return true;
        }
        return false;
    }
};

class TextField : public GuiElement {
public:
    std::string text;
    bool active = false;
    
    TextField(pair<int> pos, pair<int> s, SDL_Texture* tex);
    bool onClick(int b) override;
    bool onKey(int k) override;
    bool onText(std::string t) override;
    void preRender() override;
};

class ItemSlot : public GuiElement {
protected:
    ItemContainer* itemContainer;
public:
    ItemSlot(pair<int> pos, ItemContainer* c);
    void preRender() override;
    bool onClick(int b) override;
};

class MouseSlot : public ItemSlot {
public:
    MouseSlot();
    void update() override;
    void preRender() override;
};

class Hotbar : public GuiElement {
private:
    v(ItemContainer*) hotbarContainers;
    int* selected;
    
public:
    Hotbar(v(ItemContainer*) items, int* sel);
    void preRender() override;
    bool onScroll(int y) override;
    bool onKey(int k) override;
    bool onClick(int b) override;
};


class HealthBar : public GuiElement {
public:
    int* health;
    SDL_Texture* heart;
    HealthBar(int* h);
    void preRender() override;
};


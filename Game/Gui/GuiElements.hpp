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
#include "../Window.hpp"

class GuiManager;

class GuiElement {
protected:
    pair<int> position;
    pair<int> size;
    SDL_Texture* texture;
    SDL_Texture* hoverTexture;
    GuiElement* parent = nullptr;
    v(GuiElement*) children;
    bool check(pair<int> p);
    
    friend GuiManager;

public:
    bool alive = true;
    GuiManager* manager = nullptr;
    
    virtual bool onClick(int b) { return false; }
    virtual bool onKey(int k) { return false; }
    virtual bool onScroll(int y) { return false; }
    
    GuiElement(pair<int> pos, pair<int> s = {0,0}, SDL_Texture* tex = nullptr, SDL_Texture* tex2 = nullptr);
    
    void addGuiElement(GuiElement* gui);
    void setParent(GuiElement* gui);
    void setManager(GuiManager* m);
    bool handleEvent(SDL_Event event);
    
    void destroy();
    virtual void onDestroy() {}
    
    virtual void render();
    virtual void update();
    
};


class Widget : public GuiElement {
private:
    GuiElement* link;
public:
    Widget(pair<int> pos, pair<int> s, SDL_Texture* tex = nullptr, GuiElement* l = nullptr);
    bool onKey(int key) override;
    void onDestroy() override;
};

class TextElement : public GuiElement {
private:
    std::string text;
    bool centre;
public:
    TextElement(pair<int> pos, std::string t, bool c = false);
    void render() override;
};

class DisplayElement : public GuiElement {
private:
    int* value;
public:
    DisplayElement(pair<int> pos, int* v);
    void render() override;
};


class Button : public GuiElement {
private:
    void(*function)();
public:
    Button(pair<int> pos, pair<int> s, void(*func)(), SDL_Texture* tex = nullptr, SDL_Texture* tex2 = nullptr);
    bool onClick(int b) override;
};

class TextField : public GuiElement {
public:
    std::string s;
    bool active = false;
    
    TextField(pair<int> pos, pair<int> s);
    bool onClick(int b) override;
    bool onKey(int k) override;
};

class ItemSlot : public GuiElement {
private:
    ItemContainer* itemContainer;
public:
    ItemSlot(pair<int> pos, ItemContainer* item);
    void render() override;
    bool onClick(int b) override;
};


class Hotbar : public GuiElement {
private:
    v(ItemContainer*) hotbarContainers;
    int* selected;
    
public:
    Hotbar(v(ItemContainer*) items, int* sel);
    void render() override;
    bool onScroll(int y) override;
    bool onKey(int k) override;
    bool onClick(int b) override;
};


class HealthBar : public GuiElement {
public:
    int* health;
    SDL_Texture* heart;
    HealthBar(int* h);
    void render() override;
};


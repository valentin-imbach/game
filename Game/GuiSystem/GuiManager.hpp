//
//  GuiManager.hpp
//  Game
//
//  Created by Valentin Imbach on 23.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

#include "../Item.h"

class Gui {
public:
    bool alive = true;
    virtual void update() {};
    virtual void render() {};
    virtual bool handleEvent(SDL_Event event) { return false; };
protected:
    pair<int> position;
    pair<int> size;
    bool check(pair<int> p) {
        return (position.X-size.X/2 <= p.X) && (p.X <= position.X+size.X/2) && (position.Y-size.Y/2 <= p.Y) && (p.Y <= position.Y+size.Y/2);
    }
};

class GuiManager {
public:
    static void Init();
    static void update();
    static void render();
    static bool handleEvent(SDL_Event event);
    static void addGui(Gui*);
    static ItemContainer* mouseSlot;
    static ItemContainer* hotbarSlot;
private:
    static v(Gui*) guiStack;
};

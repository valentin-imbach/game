//
//  GuiManager.hpp
//  Game
//
//  Created by Valentin Imbach on 23.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "../tools.h"

class Gui {
public:
    bool alive = true;
    virtual void update() {};
    virtual void render() {};
    virtual bool handleEvent(SDL_Event event) { return false; };
protected:
    pair<int> position;
    pair<int> size;
};

class GuiManager {
public:
    static void Init();
    static void update();
    static void render();
    static bool handleEvent(SDL_Event event);
    static void addGui(Gui*);
private:
    static v(Gui*) guiStack;
};

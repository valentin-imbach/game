//
//  LayerManager.hpp
//  Game
//
//  Created by Valentin Imbach on 23.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "tools.h"

struct Layer {
    bool active = true;
    virtual void update() {};
    virtual void render() {};
    virtual bool handleEvent(SDL_Event event) { return false; };
};


class LayerManager {
public:
    static void Init();
    static Layer* addLayer(Layer* l);
    static void update();
    static void render();
    static bool handleEvent(SDL_Event event);
private:
    static v(Layer*) layerStack;
};


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
private:
    v(Layer*) layerStack;
public:
    LayerManager();
    Layer* addLayer(Layer* l);
    void update();
    void render();
    bool handleEvent(SDL_Event event);
};


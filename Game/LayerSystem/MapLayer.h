//
//  MapLayer.h
//  Game
//
//  Created by Valentin Imbach on 19/01/2021.
//  Copyright © 2021 Valentin Imbach. All rights reserved.
//

#pragma once
#include "../Gui/GuiManager.hpp"
#include "Map.hpp"

class MapLayer : public Layer {
public:
    
    Map* map;
    
    MapLayer(const char* path) {
        map = Map::load(path);
    }
    
    void update() override {
        map -> update();
    }
    
    void render() override {
        map -> render();
    }
    
    bool handleEvent(SDL_Event event) override {
        return map -> handleEvent(event);
    }
};

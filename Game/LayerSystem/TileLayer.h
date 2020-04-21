//
//  TileLayer.h
//  Game
//
//  Created by Valentin Imbach on 21.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "Layers.h"
#include "Map.hpp"

class TileLayer : public Layer {
public:
    TileLayer() {
        map = new Map("map.txt");
    }
    void update() override {
        map -> update();
    }
    void render() override {
        map -> render();
    }
private:
    Map* map;
};

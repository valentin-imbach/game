//
//  CameraLayer.h
//  Game
//
//  Created by Valentin Imbach on 03.07.21.
//  Copyright © 2021 Valentin Imbach. All rights reserved.
//

#pragma once
#include "../Gui/GuiManager.hpp"
#include "Map.hpp"

class CameraLayer : public Layer {
public:
    void render() override {
        Camera::render();
    }
    
    bool handleEvent(SDL_Event event) override {
        return Camera::handleEvent(event);
    }
};

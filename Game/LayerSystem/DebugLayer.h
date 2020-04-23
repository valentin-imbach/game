//
//  DebugLayer.h
//  Game
//
//  Created by Valentin Imbach on 22.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

#include "../TextManager.hpp"
#include "../Window.hpp"
#include "../Camera.hpp"

class DebugLayer : public Layer {
public:
    DebugLayer() {
        LOG("Debug Layer constructed");
    }
    void update() override {
        
    }
    void render() override {
        TextManager::drawText(std::to_string(Window::FPS).c_str(), 20, 10);
        Camera::render();
    }
private:
    
};

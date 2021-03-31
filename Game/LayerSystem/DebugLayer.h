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
    void render() override {
        if (!active) return;
        TextureManager::drawTexture(TextureManager::getTexture("grey.png"), 20, 20, 300, 200);
        std::string fps = "FPS: " + std::to_string(Window::FPS);
        std::string ents = "Entities: " + std::to_string(entityCount);
        std::string ticks = "Ticks: " + std::to_string(Window::ticks);
        TextManager::drawText(fps, {30,25});
        TextManager::drawText(ents, {30,50});
        TextManager::drawText(ticks, {30,75});
    }
    
    bool handleEvent(SDL_Event e) override {
        if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_P) {
            active = !active;
            return true;
        }
        return false;
    }
private:
    bool active = false;
};

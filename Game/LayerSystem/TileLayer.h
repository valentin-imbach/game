//
//  TileLayer.h
//  Game
//
//  Created by Valentin Imbach on 21.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "Map.hpp"
#include "Camera.hpp"
#include "Window.hpp"
#include "../GuiSystem/GuiManager.hpp"

class TileLayer : public Layer {
public:
    TileLayer() {
        map = new Map("map.txt");
        LOG("Tile Layer constructed");
    }
    void update() override {
        map -> update();
    }
    void render() override {
        map -> render();
    }
    bool handleEvent(SDL_Event event) override {
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            pair<int> pos = Camera::stog(Window::mousePos).rounded();
            int t = -1;
            if (GuiManager::hotbarSlot -> item != nullptr) {
                t = GuiManager::hotbarSlot -> item -> type;
            }
            LOG("Tile",pos,"clicked with item type",t);
            return true;
        }
        return false;
    }
private:
    Map* map;
};

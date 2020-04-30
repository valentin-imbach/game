//
//  TableGui.h
//  Game
//
//  Created by Valentin Imbach on 28.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "../ECS/TableComponent.h"

class TableGui : public Gui {
private:
    pair<int> position;
    SDL_Texture* texture;
public:
    TableGui(TableComponent* tableComponent) {
        position.X = Window::size.X/2;
        position.Y = 170;
        texture = TextureManager::loadTexture("assets/tableGui.png");
        LOG("Table Gui constructed");
    }
    void render() override {
        TextureManager::drawTexture(texture, 0, 0, 90, 32, position.X, position.Y, 270, 96, true);
    }
    bool handleEvent(SDL_Event event) override {
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_E) {
                alive = false;
            }
        }
        return false;
    }
};

//
//  GuiLayer.h
//  Game
//
//  Created by Valentin Imbach on 28.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "../Gui/GuiManager.hpp"

class GuiLayer : public Layer {
private:
    GuiManager guiManager;
public:
    void update() override {
        guiManager.update();
    }
    void render() override {
        guiManager.render();
    }
    bool handleEvent(SDL_Event event) override {
        return guiManager.handleEvent(event);
    }
    void addGuiElement(GuiElement* gui) {
        guiManager.addGuiElement(gui);
    }
};

//
//  GuiLayer.h
//  Game
//
//  Created by Valentin Imbach on 21.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

#include "../ECS/GuiComponent.h"

class GuiLayer : public Layer {
public:
    
    GuiLayer() {
        LOG("Gui Layer constructed");
    }
    
    void update() override {
        for (auto g : guiStack) {
            g -> guiUpdate();
        }
    }
    
    void render() override {
        for (auto g : guiStack) {
            g -> guiRender();
        }
    }
    
    void addGui(GuiComponent* gui) {
        guiStack.push_back(gui);
    }
    
    bool handleEvent(SDL_Event event) override {
        for (auto g : guiStack) {
            if (g -> guiHandleEvent(event)) {
                return true;
            }
        }
        return false;
    }
    
private:
    v(GuiComponent*) guiStack;
};

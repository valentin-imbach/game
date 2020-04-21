//
//  LayerManager.h
//  Game
//
//  Created by Valentin Imbach on 21.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

struct Layer {
    virtual void init() {};
    virtual void update() {};
    virtual void render() {};
    virtual bool handleEvent(SDL_Event event) { return true; };
};

class LayerManager {
public:
    
    LayerManager() {
        layerStack = std::vector<Layer*>();
    }
    
    Layer* addLayer(Layer* l) {
        layerStack.push_back(l);
        return l;
    }
    
    void update() {
        for (Layer* l: layerStack) {
            l -> update();
        }
    }
    
    void render() {
        for (Layer* l: layerStack) {
            l -> render();
        }
    }
    
    bool handleEvent(SDL_Event event) {
        for (auto i = layerStack.size()-1; i >= 0; i--) {
            if (layerStack[i] -> handleEvent(event)) {
                return true;
            }
        }
        return false;
    }
    
private:
    std::vector<Layer*> layerStack;
};


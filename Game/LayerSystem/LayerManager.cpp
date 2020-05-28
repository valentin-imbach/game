//
//  LayerManager.cpp
//  Game
//
//  Created by Valentin Imbach on 23.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "LayerManager.hpp"

LayerManager::LayerManager() {
    LOG("Layer Manager initialized");
}
    
Layer* LayerManager::addLayer(Layer* l) {
    layerStack.push_back(l);
    return l;
}
    
void LayerManager::update() {
    for (Layer* l: layerStack) {
        if (l -> active) {
            l -> update();
        }
    }
}
    
void LayerManager::render() {
    for (Layer* l: layerStack) {
        if (l -> active) {
            l -> render();
        }
    }
}
    
bool LayerManager::handleEvent(SDL_Event event) {
    if (event.key.repeat) { return false; }
    for (int i = (int) layerStack.size()-1; i >= 0; i--) {
        if (layerStack[i] -> active) {
            if (layerStack[i] -> handleEvent(event)) {
                return true;
            }
        }
    }
    return false;
}


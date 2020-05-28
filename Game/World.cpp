//
//  World.cpp
//  Game
//
//  Created by Valentin Imbach on 28.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "World.hpp"

World::World() {
    layerManager.addLayer(&entityLayer);
    layerManager.addLayer(&debugLayer);
}

void World::update() {
    guiManager.update();
    layerManager.update();
}

void World::render() {
    layerManager.render();
    guiManager.render();
}

bool World::handleEvent(SDL_Event event) {
    if (guiManager.handleEvent(event)) { return true; }
    if (layerManager.handleEvent(event)) { return true; }
    return false;
}

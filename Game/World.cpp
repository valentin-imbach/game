//
//  World.cpp
//  Game
//
//  Created by Valentin Imbach on 28.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "World.hpp"

World::World() {
    layerManager.addLayer(entityLayer = new EntityLayer());
    layerManager.addLayer(debugLayer = new DebugLayer());
    console = new Console(entityLayer);
}

void World::update() {
    layerManager.update();
}

void World::render() {
    layerManager.render();
}

bool World::handleEvent(SDL_Event event) {
    return layerManager.handleEvent(event);
}

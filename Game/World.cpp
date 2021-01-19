//
//  World.cpp
//  Game
//
//  Created by Valentin Imbach on 28.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "World.hpp"

World::World(std::string n) : name(n), mapLayer("map.txt") {
    layerManager.addLayer(&mapLayer);
    layerManager.addLayer(&entityLayer);
    layerManager.addLayer(&guiLayer);
    layerManager.addLayer(&debugLayer);
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

void World::serialize(std::fstream& stream) {
    entityLayer.serialize(stream);
}

void World::deserialize(std::fstream& stream) {
    entityLayer.deserialize(stream);
}

//
//  World.cpp
//  Game
//
//  Created by Valentin Imbach on 28.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "World.hpp"
#include "EntityFactory.hpp"

World::World(std::string n) : name(n), mapLayer("map.txt"), debugLayer(&entityLayer) {
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

void World::populate() {
    for (int x = 0; x < mapLayer.map -> size.X; x++) {
        for (int y = 0; y < mapLayer.map -> size.Y; y++) {
            if (mapLayer.map -> tiles[x][y] -> tileID == TileID::WATER) continue;
            if (!entityLayer.entityManager.isFree(x, y)) continue;
            int n = randRange(0, 30);
            if (n < 3) {
                EntityFactory::createEntity(&entityLayer.entityManager, n, {x,y});
            }
        }
    }
}

void World::serialize(std::fstream& stream) {
    entityLayer.serialize(stream);
}

void World::deserialize(std::fstream& stream) {
    entityLayer.deserialize(stream);
}

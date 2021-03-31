//
//  EntityManager.cpp
//  Game
//
//  Created by Valentin Imbach on 09.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "Components.h"

Entity* EntityManager::addEntity() {
    Entity* e = new Entity(this);
    entities.push_back(e);
    return e;
}

Entity* EntityManager::createEntity(std::fstream& stream) {
    Entity* e = new Entity(this);
    entities.push_back(e);
    e -> deserialize(stream);
    return e;
}

Entity* EntityManager::getEntity(int x, int y) {
    if (0 <= x && x < entities.size() && 0 <= y && y < gridEntities[0].size()) return gridEntities[x][y];
    return nullptr;
}

void EntityManager::update() const {
    for (Entity* e : entities) e -> update();
}

void EntityManager::render() {
    for (Entity* e : entities) e -> render();
}

void EntityManager::debugRender() const {
    for (Entity* e : entities) e -> debugRender();
}

void EntityManager::refresh() {
    for (int i = (int)entities.size()-1; i >= 0; i--) {
        if (!entities[i] -> alive) {
            delete entities[i];
            entities.erase(entities.begin() + i);
        }
    }
}

bool EntityManager::handleEvent(SDL_Event event) {
    for (Entity* e : entities)
        if (e -> handleEvent(event)) return true;
    return false;
}

void EntityManager::serialize(std::fstream &stream) {
    int count = 0;
    for (Entity* e : entities)
        if (!e -> hasTag(EntityTag::TILE)) count += 1;
    serialize_(stream, count);
    for (Entity* e : entities)
        if (!e -> hasTag(EntityTag::TILE)) e -> serialize(stream);
}

void EntityManager::deserialize(std::fstream &stream) {
    int count;
    deserialize_(stream, count);
    for (int i = 0; i < count; i++) {
        Entity* e = addEntity();
        e -> deserialize(stream);
        if (e -> hasTag(EntityTag::PLAYER)) player = e;
    }
}

bool EntityManager::isFree(int x, int y, int w, int h) const {
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++)
            if (gridEntities[x+i][y+j] != nullptr) return false;
    }
    return true;
}

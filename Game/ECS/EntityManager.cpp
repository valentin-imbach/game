//
//  EntityManager.cpp
//  Game
//
//  Created by Valentin Imbach on 09.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "ECS.h"

std::vector<Entity*>& EntityManager::getTagged(TAG tag) {
    return taggedEntities[tag];
}

Entity* EntityManager::addEntity() {
    Entity* e = new Entity(this);
    entities.push_back(e);
    return e;
}

void EntityManager::update() {
    for (Entity* e : entities) {e -> update(); }
}

void EntityManager::render() {
    for (Entity* e : getTagged(TAG::TILE)) {e -> render(); }
    for (Entity* e : getTagged(TAG::ITEM)) {e -> render(); }
    for (Entity* e : getTagged(TAG::STRUCT)) {e -> render(); }
    for (Entity* e : getTagged(TAG::PLAYER)) {e -> render(); }
}

void EntityManager::debugRender() {
    for (Entity* e : entities) {e -> debugRender(); }
}

void EntityManager::refresh() {
    for (int i = 0; i < maxTags; i++) {
        auto& v = taggedEntities[i];
        v.erase(std::remove_if(v.begin(), v.end(), [i](Entity* e) { return !(e -> active) || !(e -> hasTag((TAG)i)); }), v.end());
    }
    entities.erase(std::remove_if(std::begin(entities), std::end(entities),[](const Entity* e) { return !(e -> active); }), std::end(entities));
}

bool EntityManager::handleEvent(SDL_Event event) {
    for (Entity* e : entities) {
        if (e -> handleEvent(event)) { return true; }
    }
    return false;
}

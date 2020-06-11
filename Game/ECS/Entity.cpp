//
//  Entity.cpp
//  Game
//
//  Created by Valentin Imbach on 07.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "ECS.h"

Entity::Entity(EntityManager* m) {
    manager = m;
}

void Entity::update() {
    for (auto& c : components) { c -> update(); }
}

void Entity::render() {
    for (auto& c : components) { c -> render(); }
}

void Entity::debugRender() {
    for (auto& c : components) { c -> debugRender(); }
}

bool Entity::handleEvent(SDL_Event event) {
    for (auto& c : components) {
        if (c -> handleEvent(event)) { return true; }
    }
    return false;
}

void Entity::destroy() {
    active = false;
}

bool Entity::hasTag(TAG tag) {
    return tagBitSet[tag];
}

void Entity::addTag(TAG tag) {
    tagBitSet[(int)tag] = true;
    manager -> taggedEntities[(int)tag].push_back(this);
}

void Entity::serialize(std::fstream &stream) {
    
    //Tags
    serialize_(stream, tagBitSet);
    
    //Components
    int s = (int)components.size();
    serialize_(stream, s);
    for (auto& c : components) {
        serialize_(stream, c -> compType);
        c -> serialize(stream);
    }
}

void Entity::deserialize(std::fstream &stream) {
    
    //Tags
    deserialize_(stream, tagBitSet);
    for (int i = 0; i < tagBitSet.size(); i++) {
        if (tagBitSet[i]) {
            addTag(TAG(i));
        }
    }
    
    //Components
    int s;
    deserialize_(stream, s);
    for (int i = 0; i < s; i++) {
        ComponentType type;
        deserialize_(stream, type);
        //LOG("Desirialized type",(int)type);
        Component* comp = Component::prototypes[(int)type] -> create();
        comp -> deserialize(stream);
        addComponent(comp,type);
    }
}


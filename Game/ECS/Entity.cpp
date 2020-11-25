//
//  Entity.cpp
//  Game
//
//  Created by Valentin Imbach on 07.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "Components.h"

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

bool Entity::hasTag(EntityTag tag) {
    return tagBitSet[(int)tag];
}

void Entity::addTag(EntityTag tag) {
    tagBitSet[(int)tag] = true;
}

Entity::~Entity() {
    LOG("Entity destroyed");
    for (auto& c : components) { delete c; }
}

void Entity::serialize(std::fstream &stream) {
    
    //Tags
    serialize_(stream, tagBitSet);
    
    //Components
    int s = (int)components.size();
    serialize_(stream, s);
    for (auto& c : components) {
        serialize_(stream, c -> compType);
        int s = c -> get_size();
        serialize_(stream, s);
        c -> serialize(stream);
    }
}

void Entity::deserialize(std::fstream &stream) {
    
    //Tags
    deserialize_(stream, tagBitSet);
    for (int i = 0; i < tagBitSet.size(); i++) {
        if (tagBitSet[i]) {
            addTag(EntityTag(i));
        }
    }
    
    //Components
    int num;
    deserialize_(stream, num);
    for (int i = 0; i < num; i++) {
        ComponentType type;
        deserialize_(stream, type);
        int s;
        deserialize_(stream, s);
        //LOG("Desirialized type",(int)type);
        Component* comp = Component::prototypes[(int)type] -> create();
        comp -> deserialize(stream);
        addComponent(comp,type);
    }
}


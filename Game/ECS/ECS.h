//
//  ECS.h
//  Game
//
//  Created by Valentin Imbach on 26.03.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "tools.h"

class EntityManager;
class Entity;
class Component;

constexpr std::size_t maxComponents = 32;
using ComponentArray = std::array<Component*,maxComponents>;
using ComponentBitSet = std::bitset<maxComponents>;
using ComponentType = std::size_t;

inline ComponentType getComponentType() {
    static ComponentType lastType = 0;
    return lastType++;
}

template <typename T> inline ComponentType getComponentType() noexcept {
    static ComponentType type = getComponentType();
    return type;
}

class Component {
public:
    Entity* entity;
    
    virtual void init() {};
    virtual void update() {};
    virtual void render() {};
    virtual bool handleEvent(SDL_Event event) { return false; }
    virtual void debugRender() {};
    
    virtual ~Component() {};
};

class Entity {
    
private:
    vup(Component) components;
    ComponentArray componentArray;
    ComponentBitSet componentBitSet;
    
public:
    bool active = true;
    EntityManager* manager;
    
    Entity(EntityManager* m) {
        manager = m;
    }
    void update() {
        for (auto& c : components) { c -> update(); }
    }
    
    void render() {
        for (auto& c : components) { c -> render(); }
    }
    
    void debugRender() {
        for (auto& c : components) { c -> debugRender(); }
    }
    
    bool handleEvent(SDL_Event event) {
        for (auto& c : components) {
            if (c -> handleEvent(event)) {
                return true;
            }
        }
        return false;
    }
    
    void destroy() {
        active = false;
    }
    
    template <typename T> bool hasComponent() const {
        return componentBitSet[getComponentType<T>()];
    }
    
    template <typename T, typename... TArgs> T& addComponent(TArgs&&... mArgs) {
        if (hasComponent<T>()) {
            return getComponent<T>();
        }
        T* component( new T(std::forward<TArgs>(mArgs)...));
        component -> entity = this;
        std::unique_ptr<Component> uPtr{component};
        components.emplace_back(std::move(uPtr));
        componentArray[getComponentType<T>()] = component;
        componentBitSet[getComponentType<T>()] = true;
        component -> init();
        return *component;
    }
    
    template <typename T> T& getComponent() const {
        return *static_cast<T*>(componentArray[getComponentType<T>()]);
    }
};

class EntityManager {
public:
    v(Entity*) entities;
    vv(Entity*) gridEntities = vv(Entity*)(100,v(Entity*)(100,nullptr));

    void update() {
        for (Entity* e : entities) {e -> update(); }
    }
    void render() {
        for (Entity* e : entities) {e -> render(); }
    }
    void debugRender() {
        for (Entity* e : entities) {e -> debugRender(); }
    }
    bool handleEvent(SDL_Event event) {
        for (Entity* e : entities) {
            if (e -> handleEvent(event)) {
                return true;
            }
        }
        return false;
    }
    void refresh() {
        entities.erase(std::remove_if(std::begin(entities), std::end(entities),
        [](const Entity* e) { return !(e -> active); }), std::end(entities));
    }
    Entity* addEntity() {
        Entity* e = new Entity(this);
        entities.push_back(e);
        return e;
    }
    
    Entity* addGridEntity(pair<int> pos) {
        Entity* e = new Entity(this);
        entities.push_back(e);
        gridEntities[pos.X][pos.Y] = e;
        return e;
    }
};

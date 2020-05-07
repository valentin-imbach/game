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

inline ComponentType getNewComponentType() {
    static ComponentType lastType = 0;
    return lastType++;
}

template <typename T> inline ComponentType getComponentType() noexcept {
    static ComponentType type = getNewComponentType();
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

constexpr std::size_t maxTags = 32;
using TagBitSet = std::bitset<maxTags>;

enum TAG {
    TILE,
    STRUCT,
    ITEM,
    PLAYER
};

class Entity {
    
private:
    vup(Component) components;
    ComponentArray componentArray;
    ComponentBitSet componentBitSet;
    TagBitSet tagBitSet;
    
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
    
    bool hasTag(TAG tag) {
        return tagBitSet[tag];
    }
    
    bool hasTag(int tag) {
        return tagBitSet[tag];
    }
    
    void addTag(TAG tag);
    
    template <typename T> bool hasComponent() const {
        return componentBitSet[getComponentType<T>()];
    }
    
    template <typename T, typename... TArgs> T* addComponent(TArgs&&... mArgs) {
        assert(!hasComponent<T>());
        componentBitSet[getComponentType<T>()] = true;
        T* component = new T(this, std::forward<TArgs>(mArgs)...);
        std::unique_ptr<Component> uPtr{component};
        components.emplace_back(std::move(uPtr));
        componentArray[getComponentType<T>()] = component;
        component -> entity = this;
        return component;
    }
    
    template <typename T> T* getComponent() const {
        assert(hasComponent<T>());
        return static_cast<T*>(componentArray[getComponentType<T>()]);
    }
};

class EntityManager {
public:
    v(Entity*) entities;
    std::array<std::vector<Entity*>, maxTags> taggedEntities;
    vv(Entity*) gridEntities = vv(Entity*)(100,v(Entity*)(100,nullptr));
    
    std::vector<Entity*>& getTagged(TAG tag) {
        return taggedEntities[tag];
    }
    
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
        for (int i = 0; i < maxTags; i++) {
            auto& v = taggedEntities[i];
            v.erase(std::remove_if(v.begin(), v.end(), [i](Entity* e) { return !(e -> active) || !(e -> hasTag(i)); }), v.end());
        }
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

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

constexpr std::size_t maxTags = 32;
using TagBitSet = std::bitset<maxTags>;

enum TAG {
    TILE,
    STRUCT,
    ITEM,
    PLAYER
};

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
    virtual void serialize(std::fstream& stream) {}
    virtual void deserialize(std::fstream& stream) {}
    virtual ~Component() {};
};

class Entity {
private:
    v(Component*) components;
    ComponentArray componentArray;
    ComponentBitSet componentBitSet;
    TagBitSet tagBitSet;
    
public:
    bool active = true;
    EntityManager* manager;
    
    Entity(EntityManager* m);
    
    void update();
    void render();
    void debugRender();
    
    bool handleEvent(SDL_Event event);
    void destroy();
    
    bool hasTag(TAG tag);
    void addTag(TAG tag);
    
    void serialize(std::fstream& stream);
    void deserialize(std::fstream& stream);
    
    template <typename T> bool hasComponent() const {
        return componentBitSet[getComponentType<T>()];
    }
    
    template <typename T, typename... TArgs> T* addComponent(TArgs&&... mArgs) {
        assert(!hasComponent<T>());
        componentBitSet[getComponentType<T>()] = true;
        T* component = new T(this, std::forward<TArgs>(mArgs)...);
        components.push_back(component);
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
    
    Entity* addEntity();
    std::vector<Entity*>& getTagged(TAG tag);

    void refresh();
    void update();
    void render();
    void debugRender();
    
    bool handleEvent(SDL_Event event);
    Entity* player = nullptr;
};



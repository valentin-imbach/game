//
//  ECS.h
//  Game
//
//  Created by Valentin Imbach on 26.03.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "tools.h"
#include "../serializer.h"

class EntityManager;
class Entity;
class Component;

constexpr int maxComponents = 32;
using ComponentArray = std::array<Component*,maxComponents>;
using ComponentBitSet = std::bitset<maxComponents>;

constexpr int maxTags = 32;
using TagBitSet = std::bitset<maxTags>;

enum TAG {
    TILE,
    STRUCT,
    ITEM,
    PLAYER
};

enum class ComponentType : int {
    POSITION,
    DIRECTION,
    SIZE,
    GRID,
    HEALTH,
    TILE,
    INVENTORY,
    COLLISION,
    SPRITE,
    ITEM,
    RESOURCE,
    TABLE,
    CHEST,
    PLAYER_GUI,
    PLAYER_INPUT,
    PLAYER_ANIMATION
};

class Component {
public:
    Entity* entity;
    ComponentType compType;
    static ComponentArray prototypes;
    static void setPrototypes();
    
    virtual void init() {};
    virtual void update() {};
    virtual void render() {};
    virtual bool handleEvent(SDL_Event event) { return false; }
    virtual void debugRender() {};
    
    virtual Component* create() { return new Component(); LOG("DEFAULT CREATE"); }
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
    
    Component* addComponent(Component* component, ComponentType type) {
        assert(!componentBitSet[(int)type]);
        component -> compType = type;
        components.push_back(component);
        componentBitSet[(int)type] = true;
        componentArray[(int)type] = component;
        component -> entity = this;
        component -> init();
        return component;
    }
    
    template <typename T> bool hasComponent() const {
        return componentBitSet[(int)T::componentType];
    }
    
    template <typename T, typename... TArgs> T* addComponent(TArgs&&... mArgs) {
        assert(!hasComponent<T>());
        componentBitSet[(int)T::componentType] = true;
        T* component = new T(std::forward<TArgs>(mArgs)...);
        component -> compType = T::componentType;
        components.push_back(component);
        componentArray[(int)T::componentType] = component;
        component -> entity = this;
        component -> init();
        return component;
    }
    
    template <typename T> T* getComponent() const {
        assert(hasComponent<T>());
        return static_cast<T*>(componentArray[(int)T::componentType]);
    }

};

class EntityManager {
public:
    v(Entity*) entities;
    std::array<std::vector<Entity*>, maxTags> taggedEntities;
    vv(Entity*) gridEntities = vv(Entity*)(100,v(Entity*)(100,nullptr));
    
    Entity* addEntity();
    Entity* createEntity(std::fstream& file);
    
    std::vector<Entity*>& getTagged(TAG tag);

    void refresh();
    void update();
    void render();
    void debugRender();
    
    bool handleEvent(SDL_Event event);
    Entity* player = nullptr;
};



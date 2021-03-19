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
#include "../MessagingSystem.hpp"

class EntityManager;
class Entity;
class Component;

constexpr int maxComponents = 32;
using ComponentArray = std::array<Component*,maxComponents>;
using ComponentBitSet = std::bitset<maxComponents>;

constexpr int maxTags = 32;
using TagBitSet = std::bitset<maxTags>;

enum class EntityTag {
    TILE,
    STRUCT,
    ITEM,
    PLAYER
};

enum class ComponentType {
    POSITION,
    SIZE,
    DIRECTION,
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
    FURNACE,
    PLAYER_GUI,
    PLAYER_INPUT,
    PLAYER_ANIMATION,
    PICKUP
};

class Component : public Serializable {
public:
    Entity* entity;
    ComponentType compType;
    
    static ComponentArray prototypes;
    static void setPrototypes();
    
    virtual void init() {}
    virtual void update() {}
    virtual void render() {}
    virtual bool handleEvent(SDL_Event event) { return false; }
    virtual bool onMessage(const Message &message) { return false; }
    virtual void debugRender() {}
    
    virtual Component* create() { return new Component(); LOG("DEFAULT CREATE"); }
    
    virtual ~Component() {}
};

class Entity : public Serializable, public Observer {
private:
    v(Component*) components;
    ComponentArray componentArray;
    ComponentBitSet componentBitSet;
    TagBitSet tagBitSet;
    
public:
    bool alive = true;
    EntityManager* manager;
    
    Entity(EntityManager* m);
    ~Entity();
    
    void update();
    void render();
    void debugRender();
    
    bool handleEvent(SDL_Event event);
    bool onMessage(const Message& message) override;
    
    void destroy();
    
    bool hasTag(EntityTag tag);
    void addTag(EntityTag tag);
    
    void serialize(std::fstream& stream) override;
    void deserialize(std::fstream& stream) override;
    
    template <typename T> bool hasComponent() const {
        return componentBitSet[(int)T::componentType];
    }
    
    template <typename T> T* getComponent() const {
        assert(hasComponent<T>());
        return static_cast<T*>(componentArray[(int)T::componentType]);
    }
    
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
};

class EntityManager : public Serializable {
public:
    v(Entity*) entities;
    vv(Entity*) gridEntities = vv(Entity*)(100,v(Entity*)(100,nullptr));
    
    Entity* addEntity();
    Entity* createEntity(std::fstream& file);
    
    bool isFree(const int x, const int y, const int w = 1, const int h = 1) const;
    
    void refresh();
    void update() const;
    void render();
    void debugRender() const;
    
    void serialize(std::fstream& stream) override;
    void deserialize(std::fstream& stream) override;
    
    bool handleEvent(SDL_Event event);
    Entity* player = nullptr;
};



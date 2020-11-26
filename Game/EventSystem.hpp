//
//  EventSystem.hpp
//  Game
//
//  Created by Valentin Imbach on 26/11/2020.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "tools.h"
#include "ECS.h"

constexpr int MaxEventTypes = 10;

enum class EventType {
    INTERACTION,
    ATTACK,
    BREAK,
    PICKUP,
    KILL,
    THROW,
    INVENTORY
};

struct GameEvent {
    EventType type;
    GameEvent(EventType type);
};

struct Observer {
    void onEvent(GameEvent event);
    void subscribe(EventType type);
};

class EventManager {
public:
    static EventManager* Instance;
    EventManager();
    v(Observer*) subscribers[MaxEventTypes];
    void distribute(GameEvent event);
};

struct InteractionEvent : GameEvent {
    InteractionEvent(Entity* target) : GameEvent(EventType::INTERACTION), target(target) {}
    Entity* target;
};

struct AttackEvent : GameEvent {
    AttackEvent(Entity* target) : GameEvent(EventType::ATTACK), target(target) {}
    Entity* target;
};

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
#include "Item.hpp"

constexpr int MaxMessageTypes = 10;

enum class MessageType {
    INTERACTION,
    ATTACK,
    BREAK,
    USE_ITEM,
    PICKUP_ITEM,
    KILL,
    THROW_ITEM,
    INVENTORY,
    DAMAGE
};

struct Message {
    MessageType type;
    Message(MessageType type);
};

struct Observer {
    v(MessageType) subscriptions;
    virtual bool onMessage(Message event) = 0;
    void subscribe(MessageType type);
    void unsubscribe(MessageType type);
    ~Observer();
};

class MessageManager {
public:
    static vv(Observer*) subscribers;
    static void notify(Message message);
    static void distribute(Message message);
};

struct InteractionMessage : Message {
    InteractionMessage(Entity* a, Entity* t, Item* i = nullptr) : Message(MessageType::INTERACTION), actor(a), target(t), item(i) {}
    Entity* actor;
    Entity* target;
    Item* item;
};

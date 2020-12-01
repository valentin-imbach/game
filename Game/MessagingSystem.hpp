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
    ITEM_USE,
    ITEM_PICKUP,
    KILL,
    ITEM_THROW,
    SPAWN_ITEM,
    INVENTORY,
    DAMAGE
};

struct Message {
    MessageType type;
    Message(MessageType type);
};

struct Observer {
    v(MessageType) subscriptions;
    virtual bool onMessage(const Message &message) = 0;
    void subscribe(MessageType type);
    void unsubscribe(MessageType type);
    ~Observer();
};

class MessageManager {
public:
    static vv(Observer*) subscribers;
    static void notify(const Message &message);
    static void distribute(const Message &message);
};

struct InventoryMessage : public Message {
    InventoryMessage() : Message(MessageType::INVENTORY) {}
};

struct ItemThrowMessage : public Message {
    ItemThrowMessage() : Message(MessageType::ITEM_THROW) {}
};

struct InteractionMessage : public Message {
    InteractionMessage(Entity* a, Entity* t, Item* i = nullptr) : Message(MessageType::INTERACTION), actor(a), target(t), item(i) {}
    Entity* actor;
    Entity* target;
    Item* item;
};

struct SpawnItemMessage : public Message {
    SpawnItemMessage(Item* item, pair<float> pos) : Message(MessageType::SPAWN_ITEM), item(item), position(pos) {}
    Item* item;
    pair<float> position;
};

//
//  EventSystem.hpp
//  Game
//
//  Created by Valentin Imbach on 26/11/2020.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "tools.h"
#include "Item.hpp"

class Entity;

enum class MessageType {
    INTERACTION,
    ATTACK,
    PLACE,
    BREAK,
    ITEM_USE,
    ITEM_PICKUP,
    KILL,
    ITEM_THROW,
    SPAWN_ITEM,
    INVENTORY,
    DAMAGE,
    PRINT,
    
    MAX
};

struct Observer;

struct Message {
    MessageType type;
    Observer* target;
    Message(MessageType type, Observer* target = nullptr) : type(type), target(target) {}
};

struct Observer {
    v(MessageType) subscriptions;
    virtual bool onMessage(const Message &message) = 0;
    bool isSubscribed(MessageType type);
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
    pair<int> position;
    InventoryMessage(pair<int> pos = Window::size/2) : Message(MessageType::INVENTORY), position(pos) {}
};

struct ItemThrowMessage : public Message {
    ItemThrowMessage() : Message(MessageType::ITEM_THROW) {}
};

struct InteractionMessage : public Message {
    Item* item;
    InteractionMessage(Observer* t, Item* i = nullptr) : Message(MessageType::INTERACTION, t), item(i) {}
};

struct AttackMessage : public Message {
    Item* item;
    AttackMessage(Observer* t, Item* i) : Message(MessageType::ATTACK, t), item(i) {}
};

struct SpawnItemMessage : public Message {
    Item* item;
    pair<float> position;
    SpawnItemMessage(Item* item, pair<float> pos) : Message(MessageType::SPAWN_ITEM), item(item), position(pos) {}
};

struct PlaceMessage : public Message {
    Entity* entity;
    PlaceMessage(Entity* e) : Message(MessageType::PLACE), entity(e) {}
};

struct BreakMessage : public Message {
    Entity* entity;
    BreakMessage(Entity* e) : Message(MessageType::BREAK), entity(e) {}
};

struct PrintMessage : public Message {
    std::string text;
    PrintMessage(std::string t) : Message(MessageType::PRINT), text(t) {}
};

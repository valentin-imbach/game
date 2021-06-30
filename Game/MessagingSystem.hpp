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
    WORLD_CLICK,
    INTERACTION,
    
    PLACE,
    BREAK,
    
    ITEM_USE,
    ITEM_PICKUP,
    ITEM_THROW,
    
    DAMAGE,
    KILL,
    KILL_PLAYER,
    DEATH,
    
    GIVE,
    SPAWN_ITEM,
    INVENTORY,

    PRINT,

    QUIT,
    REFRESH,
    TOGGLE_GOD,
    GRID_PLACE,
    TELEPORT,
    SOUND,
    
    MAX
};

struct Observer;

struct Message {
    MessageType type;
    Message(MessageType type) : type(type) {}
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
    pair<int> offset;
    InventoryMessage(pair<int> off = 0) : Message(MessageType::INVENTORY), offset(off) {}
};

struct ItemThrowMessage : public Message {
    ItemThrowMessage() : Message(MessageType::ITEM_THROW) {}
};

struct WorldClickMessage : public Message {
    bool attack;
    pair<float> position;
    WorldClickMessage(pair<float> pos, bool a = false) : Message(MessageType::WORLD_CLICK), attack(a), position(pos) {}
};

struct InteractionMessage : public Message {
    Item* item;
    bool attack;
    pair<float> position;
    InteractionMessage(pair<float> pos, bool a, Item* i) : Message(MessageType::INTERACTION), item(i), attack(a), position(pos) {}
};

struct QuitMessage : public Message {
    QuitMessage() : Message(MessageType::QUIT) {}
};

struct RefreshMessage : public Message {
    RefreshMessage() : Message(MessageType::REFRESH) {}
};

struct KillPlayerMessage : public Message {
    KillPlayerMessage() : Message(MessageType::KILL_PLAYER) {}
};

struct ToggleGodMessage : public Message {
    ToggleGodMessage() : Message(MessageType::TOGGLE_GOD) {}
};

struct SpawnItemMessage : public Message {
    Item* item;
    pair<float> position;
    SpawnItemMessage(Item* item, pair<float> pos) : Message(MessageType::SPAWN_ITEM), item(item), position(pos) {}
};

struct GridPlaceMessage : public Message {
    Entity* entity;
    GridPlaceMessage(Entity* e) : Message(MessageType::GRID_PLACE), entity(e) {}
};

struct PlaceMessage : public Message {
    int n;
    PlaceMessage(int n) : Message(MessageType::PLACE), n(n) {}
};

struct BreakMessage : public Message {
    Entity* entity;
    BreakMessage(Entity* e) : Message(MessageType::BREAK), entity(e) {}
};

struct PrintMessage : public Message {
    std::string text;
    PrintMessage(std::string t) : Message(MessageType::PRINT), text(t) {}
};

struct GiveMessage : public Message {
    Item* item;
    GiveMessage(Item* i) : Message(MessageType::GIVE), item(i) {}
};

struct TeleportMessage : public Message {
    pair<int> location;
    TeleportMessage(pair<int> l) : Message(MessageType::TELEPORT), location(l) {}
};

struct SoundMessage : public Message {
    const char* path;
    SoundMessage(const char* p) : Message(MessageType::SOUND), path(p) {}
};

struct PickupMessage : public Message {
    Item* item;
    PickupMessage(Item* i) : Message(MessageType::ITEM_PICKUP), item(i) {}
};

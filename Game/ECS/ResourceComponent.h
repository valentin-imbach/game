//
//  ResourceComponent.h
//  Game
//
//  Created by Valentin Imbach on 28.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "../Item.hpp"
#include "../ResoureTypes.hpp"

class ResourceComponent : public Component {
public:
    static ComponentType componentType;
    SpriteComponent* spriteComponent;
    PositionComponent* positionCompenent;
    
    int type;

    ResourceComponent(int t = 0) : type(t) {}
    
    void init() override {
        spriteComponent = entity -> getComponent<SpriteComponent>();
        positionCompenent = entity -> getComponent<PositionComponent>();
        pair<int> pos = ResourceType::types[type] -> position;
        pos.X += randRange(0, ResourceType::types[type] -> vars);
        pos.Y -= ResourceType::types[type] -> size.Y - 1;
        spriteComponent -> sprite = Sprite(TextureManager::getTexture("sprites.png"), pos, ResourceType::types[type] -> size);
        spriteComponent -> height = ResourceType::types[type] -> height;
        entity -> subscribe(MessageType::ATTACK);
    }
    
    bool onMessage(const Message& message) override {
        if (message.type == MessageType::ATTACK) {
            const AttackMessage &msg = static_cast<const AttackMessage&>(message);
            if (msg.item && msg.item -> itemID == ResourceType::types[type] -> tool) onBreak();
            return true;
        }
        return false;
    }
    
    void onBreak() {
        for (Loot l : ResourceType::types[type] -> loot.table)
            MessageManager::notify(SpawnItemMessage(l.createItem(),positionCompenent -> position));
        entity -> destroy();
    }
    
    Component* create() override {
        return new ResourceComponent();
    }
    
    SERIALIZE(type);
    
};

//
//  PickupComponent.h
//  Game
//
//  Created by Valentin Imbach on 29/12/2020.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "../Item.hpp"

class PickupComponent : public Component {
public:
    static ComponentType componentType;
    SpriteComponent* spriteComponent;
    
    int type;
    
    PickupComponent(int t = 0) : type(t) {}
    
    void init() override {
        spriteComponent = entity -> getComponent<SpriteComponent>();
        spriteComponent -> sprite = Sprite(TextureManager::getTexture("pickups.png"),{randRange(0, 3),type});
    }
    
    bool onMessage(const Message& message) override {
        if (message.type == MessageType::INTERACTION_ITEM) {
            const InteractionItemMessage &msg = static_cast<const InteractionItemMessage&>(message);
            if (msg.attack) return false;
            ItemStack* stack;
            if (type == 0) stack = new ItemStack(ItemID::BRANCH);
            else if (type == 1) stack = new ItemStack(ItemID::STONE);
            else return false;
            MessageManager::notify(GiveMessage(stack));
            entity -> destroy();
            return true;
        }
        return false;
    }
    
    Component* create() override {
        return new PickupComponent();
    }
    
};

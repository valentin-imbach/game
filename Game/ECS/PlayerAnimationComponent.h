//
//  PlayerAnimationComponent.h
//  Game
//
//  Created by Valentin Imbach on 27.03.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "Components.h"

class PlayerAnimationComponent : public Component {
public:
    static ComponentType componentType;
    SpriteComponent *spriteComponent;
    DirectionComponent *directionComponent;
    PlayerInputComponent *playerInputComponent;
    
    void init() override {
        spriteComponent = entity -> getComponent<SpriteComponent>();
        spriteComponent -> height = 1;
        directionComponent = entity -> getComponent<DirectionComponent>();
        playerInputComponent = entity -> getComponent<PlayerInputComponent>();
        
        spriteComponent -> sprite = Sprite(TextureManager::getTexture("new_character.png"), {0,0}, {1,2}, 6, {0,2}, 5);
    }
    
    void update() override {
        spriteComponent -> sprite.anker.X = directionComponent -> direction;
        spriteComponent -> sprite.running = playerInputComponent -> walking;
    }
    
    Component* create() override {
        return new PlayerAnimationComponent();
    }
    
};

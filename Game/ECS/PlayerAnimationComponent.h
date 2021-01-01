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
        directionComponent = entity -> getComponent<DirectionComponent>();
        playerInputComponent = entity -> getComponent<PlayerInputComponent>();
        
        spriteComponent -> sprite = Sprite(TextureManager::getTexture("new_character.png"),{0,0},{1,2});
    }
    
    void update() override {
        int frame = (playerInputComponent -> ticks + 4)/5 ;
        spriteComponent -> sprite.position.X = directionComponent -> direction;
        spriteComponent -> sprite.position.Y = (frame % 6) * 2;
    }
    
    Component* create() override {
        return new PlayerAnimationComponent();
    }
    
};

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
    SpriteComponent *spriteComponent;
    DirectionComponent *directionComponent;
    PlayerInputComponent *playerInputComponent;
    
    PlayerAnimationComponent() {}
    PlayerAnimationComponent(Entity* entity) {
        spriteComponent = entity -> addComponent<SpriteComponent>("character2.png",0,0,1,2);
        directionComponent = entity -> getComponent<DirectionComponent>();
        playerInputComponent = entity -> getComponent<PlayerInputComponent>();
        spriteComponent -> offset = 1;
    }
    
    void update() override {
        int frame = (playerInputComponent -> ticks + 3)/4 ;
        spriteComponent -> sprite.position.X = directionComponent -> direction;
        spriteComponent -> sprite.position.Y = (frame % 6) * 2;
    }
    
};

//
//  PlayerAnimationComponent.h
//  Game
//
//  Created by Valentin Imbach on 27.03.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

class PlayerAnimationComponent : public Component {
public:
    
    SpriteComponent *spriteComponent;
    DirectionComponent *directionComponent;
    PlayerInputComponent *playerInputComponent;
    
    void init() override {
        
        spriteComponent = &entity -> addComponent<SpriteComponent>();
        directionComponent = &entity -> addComponent<DirectionComponent>();
        playerInputComponent = &entity -> addComponent<PlayerInputComponent>();
        
        spriteComponent -> texture = TextureManager::loadTexture("assets/character2.png");
        spriteComponent -> src.w = 16;
        spriteComponent -> src.h = 32;
        spriteComponent -> offset = 1;
    }
    
    void update() override {
        int frame = (playerInputComponent -> ticks + 3)/4 ;
        spriteComponent -> src.x = (directionComponent -> direction) * 16;
        spriteComponent -> src.y = (frame % 6) * 32;
    }
    
};

//
//  GameLayer.h
//  Game
//
//  Created by Valentin Imbach on 21.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

#include "Components.h"

class EntityLayer : public Layer {
private:
    EntityManager entityManager;
public:
    Entity* player;
    EntityLayer() {
        
        player = entityManager.addEntity();
        //Entity& wall = entityManager.addEntity();
        
        player -> addComponent<PositionComponent>(50,50);
        player -> addComponent<DirectionComponent>();
        player -> addComponent<InventoryComponent>(9,5);
        player -> addComponent<SpriteComponent>();
        player -> addComponent<PlayerInputComponent>();
        player -> addComponent<PlayerAnimationComponent>();
        player -> addComponent<CollisionComponent>(0.4,0.3,0.4,0.2);
        player -> addComponent<PlayerGuiComponent>();
        
        player -> getComponent<InventoryComponent>().items[0][0] = new ItemStack(0,5);
        player -> getComponent<InventoryComponent>().items[8][4] = new ItemStack(1,2);
        player -> getComponent<InventoryComponent>().items[0][4] = new ItemStack(2,77);
        player -> getComponent<InventoryComponent>().items[8][0] = new ItemStack(3,5);
        
        //wall.addComponent<GridComponent>(3,3);
        //wall.addComponent<SpriteComponent>();
        
        LOG("Entity Layer constructed");
    }
    void update() override {
        entityManager.refresh();
        entityManager.update();
    }
    void render() override {
        entityManager.render();
        entityManager.debugRender();
    }
    
    bool handleEvent(SDL_Event event) override {
        return entityManager.handleEvent(event);
    }
};

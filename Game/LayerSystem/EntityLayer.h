//
//  GameLayer.h
//  Game
//
//  Created by Valentin Imbach on 21.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

#include "../ECS/Components.h"
#include "../CollisionManager.h"
#include "../GuiSystem/Guis.h"
#include "../Window.hpp"

class EntityLayer : public Layer {
private:
    EntityManager entityManager;
public:
    Entity* player;
    EntityLayer() {
        player = entityManager.addEntity();
        
        player -> addComponent<PositionComponent>(50,50);
        player -> addComponent<DirectionComponent>();
        
        player -> addComponent<InventoryComponent>(9,5);
        player -> getComponent<InventoryComponent>().itemSlots[0][0].item = new Tool(0);
        player -> getComponent<InventoryComponent>().itemSlots[8][4].item = new ItemStack(6,2);
        player -> getComponent<InventoryComponent>().itemSlots[0][4].item = new ItemStack(7,5);
        player -> getComponent<InventoryComponent>().itemSlots[8][0].item = new Tool(1);
        
        player -> addComponent<PlayerInputComponent>();
        player -> addComponent<PlayerAnimationComponent>();
        player -> addComponent<CollisionComponent>(0.4,0.3,0.4,0.2);
        player -> addComponent<PlayerGuiComponent>();
        
        Entity* rock = entityManager.addGridEntity(53,53);
        Entity* table = entityManager.addGridEntity(53,47);
        
        rock -> addComponent<GridComponent>(53,53);
        rock -> addComponent<SpriteComponent>("assets/nature.png");
        rock -> addComponent<ResourceComponent>(1);
        
        table -> addComponent<GridComponent>(53,47);
        table -> addComponent<SpriteComponent>("assets/table.png");
        table -> addComponent<TableComponent>();
        
        Entity* item = entityManager.addEntity();
        item -> addComponent<ItemComponent>(47,47,new ItemStack(8,1));
        
        
        LOG("Entity Layer constructed");
    }
    void update() override {
        entityManager.refresh();
        entityManager.update();
        for (auto& e : entityManager.entities) {
            if (e -> hasComponent<CollisionComponent>() && e != player) {
                Collider a = player -> getComponent<CollisionComponent>().collider;
                Collider b = e -> getComponent<CollisionComponent>().collider;
                if (CollisionManager::AABB(a,b)) {
                    player -> getComponent<PlayerInputComponent>().setBack();
                }
            }
        }
    }
    void render() override {
        std::sort(entityManager.entities.begin(), entityManager.entities.end() , [](const Entity* a, const Entity* b) {
            if (!a -> hasComponent<PositionComponent>() || !b -> hasComponent<PositionComponent>()) { return true; }
            return a -> getComponent<PositionComponent>().pos.Y < b -> getComponent<PositionComponent>().pos.Y;
        });
        entityManager.render();
        entityManager.debugRender();
    }
    
    bool handleEvent(SDL_Event event) override {
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            pair<int> pos = Camera::stog(Window::mousePos).rounded();
            Entity* entity = entityManager.gridEntities[pos.X][pos.Y];
            if (entity == nullptr) {
                return false;
            }
            int t = -1;
            Item* item = GuiManager::hotbarSlot -> item;
            if (item != nullptr) {
                t = item -> type;
                if (entity -> hasComponent<ResourceComponent>()) {
                    if (entity -> getComponent<ResourceComponent>().mine(item)) {
                        entityManager.gridEntities[pos.X][pos.Y] = nullptr;
                    }
                }
            }
            if (entity -> hasComponent<TableComponent>()) {
                GuiManager::addGui(new InventoryGui(&(player -> getComponent<InventoryComponent>()),Window::size.X/2,Window::size.Y/2));
                GuiManager::addGui(new TableGui(&(entity -> getComponent<TableComponent>())));
            }
            LOG("Entity",pos,"clicked with item type",t);
            return true;
        }
        return entityManager.handleEvent(event);
    }
};

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
#include "../ResoureTypes.hpp"

class EntityLayer : public Layer, public Observer {
public:
    EntityManager entityManager;
    Entity* player;
    EntityLayer() {
        ResourceType::setTypes();
        Component::setPrototypes();
        Item::setTemplates();
        
        subscribe(MessageType::SPAWN_ITEM);
        subscribe(MessageType::PLACE);
        subscribe(MessageType::BREAK);
        subscribe(MessageType::INTERACTION_ITEM);
        
        LOG("Entity Layer constructed");
    }
    
    /*
    void serialize(std::fstream& stream) override {
        entityManager.serialize(stream);
    }
    
    void deserialize(std::fstream& stream) override {
        entityManager.deserialize(stream);
        player = entityManager.player;
    }
     */
    
    void update() override {
        
        entityManager.refresh();
        entityManager.update();
        entityCount = (int)entityManager.entities.size();
        
        for (auto* e : entityManager.entities) {
            if (!e -> hasTag(EntityTag::ITEM)) {
                continue;
            }
            Collider a = player -> getComponent<CollisionComponent>() -> collider;
            Collider b = e -> getComponent<CollisionComponent>() -> collider;
            if (CollisionManager::AABB(a,b)) {
                player -> getComponent<InventoryComponent>() -> addItem(e -> getComponent<ItemComponent>() -> item);
                e -> alive = false;
            }
        }
    }
    
    void render() override {
        std::sort(entityManager.entities.begin(), entityManager.entities.end() , [](const Entity* a, const Entity* b) {
            if (a == nullptr || b == nullptr || !a -> hasComponent<PositionComponent>() || !b -> hasComponent<PositionComponent>()) { return true; }
            return a -> getComponent<PositionComponent>() -> position.Y < b -> getComponent<PositionComponent>() -> position.Y;
            return true;
        });
        
        entityManager.render();
        entityManager.debugRender();
        Camera::render();
    }
    
    bool onMessage(const Message &message) override {
        if (message.type == MessageType::SPAWN_ITEM) {
            const SpawnItemMessage &msg = static_cast<const SpawnItemMessage&>(message);
            Entity* e = entityManager.addEntity();
            e -> addComponent<PositionComponent>(msg.position);
            e -> addComponent<CollisionComponent>();
            e -> addComponent<ItemComponent>(msg.item);
            
        } else if (message.type == MessageType::PLACE) {
            const PlaceMessage &msg = static_cast<const PlaceMessage&>(message);
            GridComponent* grid = msg.entity -> getComponent<GridComponent>();
            if (!grid) return false;
            for (int i = 0; i < grid -> size.X; i++) {
                for (int j = 0; j < grid -> size.Y; j++) {
                    Entity* old = entityManager.gridEntities[grid -> anchor.X + i][grid -> anchor.Y + j];
                    if (old != nullptr) MessageManager::notify(BreakMessage(old));
                    entityManager.gridEntities[grid -> anchor.X + i][grid -> anchor.Y + j] = msg.entity;
                }
            }
            return true;
            
        } else if (message.type == MessageType::BREAK) {
            const BreakMessage &msg = static_cast<const BreakMessage&>(message);
            GridComponent* grid = msg.entity -> getComponent<GridComponent>();
            PositionComponent* pos = msg.entity -> getComponent<PositionComponent>();
            if (!grid || !pos) return false;
            for (int i = 0; i < grid -> size.X; i++) {
                for (int j = 0; j < grid -> size.Y; j++) {
                    entityManager.gridEntities[pos -> position.X + i][pos -> position.Y + j] = nullptr;
                }
            }
            return true;
            
        } else if (message.type == MessageType::INTERACTION_ITEM) {
            const InteractionItemMessage &msg = static_cast<const InteractionItemMessage&>(message);
            pair<int> pos = (msg.position).rounded();
            Entity* entity = entityManager.gridEntities[pos.X][pos.Y];
            if (entity == nullptr) return false;
            entity -> onMessage(message);
        }
        return false;
    }
    
    bool handleEvent(SDL_Event event) override {
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            /*
            pair<int> pos = Camera::stog(Window::mousePos).rounded();
            Entity* entity = entityManager.gridEntities[pos.X][pos.Y];
            if (entity == nullptr) return false;
            Item* item = player -> getComponent<PlayerGuiComponent>() -> getSelectedItem() -> item;
             */
            if (event.button.button == SDL_BUTTON_RIGHT) {
                MessageManager::notify(InteractionMessage(Camera::stog(Window::mousePos)));
                return true;
            } else if (event.button.button == SDL_BUTTON_LEFT) {
                MessageManager::notify(InteractionMessage(Camera::stog(Window::mousePos), true));
                return true;
            }
        }
        return player -> handleEvent(event);
    }
};

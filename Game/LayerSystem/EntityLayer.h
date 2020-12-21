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
        
        loadMap("map.txt");
        
        subscribe(MessageType::SPAWN_ITEM);
        subscribe(MessageType::PLACE);
        subscribe(MessageType::BREAK);
        
        LOG("Entity Layer constructed");
    }
    
    void serialize(std::fstream& stream) override {
        entityManager.serialize(stream);
    }
    
    void deserialize(std::fstream& stream) override {
        entityManager.deserialize(stream);
        player = entityManager.player;
    }
    
    void loadMap(const char* path) {
        std::ifstream file;
        file.open(path);
        if (!file) {
            ERROR("Map file not found");
            file.close();
            return;
        }
        
        pair<int> size;
        file >> size.X;
        file >> size.Y;
        
        TileComponent* tileComps[size.X][size.Y];
        
        for (int y = 0; y < size.Y; y++) {
            std::string s;
            file >> s;
            for (int x = 0; x < size.X; x++) {
                TileID id = WATER;
                switch (s[x]) {
                    case '0':
                        id = WATER;
                        break;
                    case '1':
                        id = SAND;
                        break;
                    case '2':
                        id = STONE;
                        break;
                    case '3':
                        id = GRASS;
                        break;
                }
                Entity* e = entityManager.addEntity();
                tileComps[x][y] = e -> addComponent<TileComponent>(pair<int>(x,y),id);
            }
        }
        
        for (int y = 1; y < size.Y-1; y++) {
            for (int x = 1; x < size.X-1; x++) {
                tileComps[x][y] -> neig[0] = tileComps[x+1][y];
                tileComps[x][y] -> neig[1] = tileComps[x+1][y-1];
                tileComps[x][y] -> neig[2] = tileComps[x][y-1];
                tileComps[x][y] -> neig[3] = tileComps[x-1][y-1];
                tileComps[x][y] -> neig[4] = tileComps[x-1][y];
                tileComps[x][y] -> neig[5] = tileComps[x-1][y+1];
                tileComps[x][y] -> neig[6] = tileComps[x][y+1];
                tileComps[x][y] -> neig[7] = tileComps[x+1][y+1];
                tileComps[x][y] -> updateStyle();
            }
        }
        
        file.close();
        LOG("Map loaded from",path);
    }
    
    void update() override {
        
        entityManager.refresh();
        entityManager.update();
        
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
        /*
        std::sort(entityManager.entities.begin(), entityManager.entities.end() , [](const Entity* a, const Entity* b) {
            if (a == nullptr || b == nullptr || !a -> hasComponent<PositionComponent>() || !b -> hasComponent<PositionComponent>()) { return true; }
            return a -> getComponent<PositionComponent>() -> position.Y < b -> getComponent<PositionComponent>() -> position.Y;
            return true;
        });
        */
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
            PositionComponent* pos = msg.entity -> getComponent<PositionComponent>();
            if (!grid || !pos) { return false; }
            for (int i = 0; i < grid -> size.X; i++) {
                for (int j = 0; j < grid -> size.Y; j++) {
                    Entity* old = entityManager.gridEntities[pos -> position.X + i][pos -> position.Y + j];
                    if (old != nullptr) { MessageManager::notify(BreakMessage(old)); }
                    entityManager.gridEntities[pos -> position.X + i][pos -> position.Y + j] = msg.entity;
                }
            }
            return true;
        } else if (message.type == MessageType::BREAK) {
            const BreakMessage &msg = static_cast<const BreakMessage&>(message);
            GridComponent* grid = msg.entity -> getComponent<GridComponent>();
            PositionComponent* pos = msg.entity -> getComponent<PositionComponent>();
            if (!grid || !pos) { return false; }
            for (int i = 0; i < grid -> size.X; i++) {
                for (int j = 0; j < grid -> size.Y; j++) {
                    entityManager.gridEntities[pos -> position.X + i][pos -> position.Y + j] = nullptr;
                }
            }
            return true;
        }
        return false;
    }
    
    bool handleEvent(SDL_Event event) override {
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            pair<int> pos = Camera::stog(Window::mousePos).rounded();
            Entity* entity = entityManager.gridEntities[pos.X][pos.Y];
            if (entity == nullptr) { return false; }
            Item* item = player -> getComponent<PlayerGuiComponent>() -> getSelectedItem() -> item;
            if (event.button.button == SDL_BUTTON_RIGHT) {
                MessageManager::notify(InteractionMessage(entity, item));
            } else if (event.button.button == SDL_BUTTON_LEFT) {
                MessageManager::notify(AttackMessage(entity, item));
            }
        }
        return player -> handleEvent(event);
    }
};

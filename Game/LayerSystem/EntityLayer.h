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
#include "../Window.hpp"
#include "../ResoureTypes.hpp"

class EntityLayer : public Layer {
public:
    EntityManager entityManager;
    Entity* player;
    EntityLayer() {
        
        ResourceType::setTypes();
        Component::setPrototypes();
        loadMap("map.txt");
        
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
                e -> active = false;
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
    
    bool handleEvent(SDL_Event event) override {
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            pair<int> pos = Camera::stog(Window::mousePos).rounded();
            Entity* entity = entityManager.gridEntities[pos.X][pos.Y];
            if (entity == nullptr) { return false; }
            return entity -> handleEvent(event);
        }
        return player -> handleEvent(event);
    }
};

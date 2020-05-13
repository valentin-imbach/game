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
        
        loadMap("map.txt");
        
        player = entityManager.addEntity();
        player -> addTag(TAG::PLAYER);
        
        player -> addComponent<PositionComponent>(pair<float>(50,50));
        player -> addComponent<DirectionComponent>();
        
        player -> addComponent<InventoryComponent>(9,5);
        player -> getComponent<InventoryComponent>() -> itemSlots[0][0].item = new Tool(0);
        player -> getComponent<InventoryComponent>() -> itemSlots[8][4].item = new ItemStack(6,2);
        player -> getComponent<InventoryComponent>() -> itemSlots[0][4].item = new ItemStack(7,5);
        player -> getComponent<InventoryComponent>() -> itemSlots[8][0].item = new Tool(1);
        
        player -> addComponent<PlayerGuiComponent>();
        player -> addComponent<PlayerInputComponent>();
        player -> addComponent<PlayerAnimationComponent>();
        player -> addComponent<CollisionComponent>(0.4,0.3,0.4,0.2);
        
        Entity* rock = entityManager.addEntity();
        rock -> addComponent<ResourceComponent>(pair<int>(53,53),1,"assets/nature.png");

        Entity* table = entityManager.addEntity();
        table -> addComponent<TableComponent>(pair<int>(53,47));
        table -> addComponent<SpriteComponent>("assets/table.png");
        
        Entity* item = entityManager.addEntity();
        item -> addComponent<ItemComponent>(pair<float>(47,47),new ItemStack(8,1));
        
        
        LOG("Entity Layer constructed");
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
        
        for (auto* e : entityManager.getTagged(TAG::ITEM)) {
            Collider a = player -> getComponent<CollisionComponent>() -> collider;
            Collider b = e -> getComponent<CollisionComponent>() -> collider;
            if (CollisionManager::AABB(a,b)) {
                player -> getComponent<InventoryComponent>() -> addItem(e -> getComponent<ItemComponent>() -> item);
                e -> active = false;
            }
        }
        
        for (auto* e : entityManager.getTagged(TAG::STRUCT)) {
            Collider a = player -> getComponent<CollisionComponent>() -> collider;
            Collider b = e -> getComponent<CollisionComponent>() -> collider;
            if (CollisionManager::AABB(a,b)) {
                player -> getComponent<PlayerInputComponent>() -> setBack();
            }
        }
    }
    
    void render() override {

        /*std::sort(entityManager.entities.begin(), entityManager.entities.end() , [](const Entity* a, const Entity* b) {
            if (a == nullptr || b == nullptr || !a -> hasComponent<PositionComponent>() || !b -> hasComponent<PositionComponent>()) { return true; }
            return a -> getComponent<PositionComponent>() -> position.Y < b -> getComponent<PositionComponent>() -> position.Y;
        });*/
        entityManager.render();
        //entityManager.debugRender();
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
                    if (entity -> getComponent<ResourceComponent>() -> mine(item)) {
                        entityManager.gridEntities[pos.X][pos.Y] = nullptr;
                    }
                }
            }
            if (entity -> hasComponent<TableComponent>()) {
                //GuiManager::addGui(new InventoryGui(player -> getComponent<InventoryComponent>(),Window::size.X/2,Window::size.Y/2));
                //GuiManager::addGui(new TableGui(entity -> getComponent<TableComponent>()));
            }
            LOG("Entity",pos,"clicked with item type",t);
            return true;
        }
        return entityManager.handleEvent(event);
    }
};

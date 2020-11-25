//
//  ResourceComponent.h
//  Game
//
//  Created by Valentin Imbach on 28.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "../Item.hpp"
#include "../ResoureTypes.hpp"

class ResourceComponent : public Component {
public:
    static ComponentType componentType;
    SpriteComponent* spriteComponent;
    GridComponent* gridComponent;
    
    int type;
    
    ItemID tool;
    LootTable loot;

    ResourceComponent(int t = 0) {
        type = t;
    }
    
    void init() override {
        loot = ResourceType::types[type] -> loot;
        tool = ResourceType::types[type] -> tool;
        
        spriteComponent = entity -> getComponent<SpriteComponent>();
        pair<int> pos = ResourceType::types[type] -> position;
        pos.X += randRange(0, ResourceType::types[type] -> variations);
        pos.Y -= ResourceType::types[type] -> size.Y - 1;
        spriteComponent -> sprite = Sprite(TextureManager::getTexture("sprites.png"), pos, ResourceType::types[type] -> size);
        
        gridComponent = entity -> getComponent<GridComponent>();
        gridComponent -> size = ResourceType::types[type] -> size;
    }
    
    bool handleEvent(SDL_Event event) override {
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                if (entity -> manager -> player -> getComponent<PlayerGuiComponent>() -> getSelectedItem() -> item -> itemID == tool) {
                    for (Loot l : loot.table) {
                        Entity* item = entity -> manager -> addEntity();
                        item -> addComponent<PositionComponent>(entity -> getComponent<PositionComponent>() -> position);
                        item -> addComponent<CollisionComponent>();
                        item -> addComponent<ItemComponent>(l.createItem());
                    }
                    entity -> destroy();
                    return true;
                }
            }
        }
        return false;
    }
    
    Component* create() override {
        return new ResourceComponent();
    }
    
    SERIALIZE(type);
    
};

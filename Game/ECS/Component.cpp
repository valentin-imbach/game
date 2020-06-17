//
//  Component.cpp
//  Game
//
//  Created by Valentin Imbach on 02.06.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "ECS.h"
#include "Components.h"

ComponentArray Component::prototypes;

void Component::setPrototypes() {
    prototypes[(int)ComponentType::POSITION] = new PositionComponent();
    prototypes[(int)ComponentType::DIRECTION] = new DirectionComponent();
    prototypes[(int)ComponentType::HEALTH] = new HealthComponent();
    prototypes[(int)ComponentType::GRID] = new GridComponent();
    prototypes[(int)ComponentType::TILE] = new TileComponent();
    prototypes[(int)ComponentType::SPRITE] = new SpriteComponent();
    prototypes[(int)ComponentType::INVENTORY] = new InventoryComponent();
    prototypes[(int)ComponentType::COLLISION] = new CollisionComponent();
    prototypes[(int)ComponentType::ITEM] = new ItemComponent();
    prototypes[(int)ComponentType::RESOURCE] = new ResourceComponent();
    prototypes[(int)ComponentType::PLAYER_GUI] = new PlayerGuiComponent();
    prototypes[(int)ComponentType::PLAYER_INPUT] = new PlayerInputComponent();
    prototypes[(int)ComponentType::PLAYER_ANIMATION] = new PlayerAnimationComponent();
    prototypes[(int)ComponentType::CHEST] = new ChestComponent();
    prototypes[(int)ComponentType::TABLE] = new TableComponent();
}


ComponentType PositionComponent::componentType = ComponentType::POSITION;
ComponentType DirectionComponent::componentType = ComponentType::DIRECTION;
ComponentType GridComponent::componentType = ComponentType::GRID;
ComponentType HealthComponent::componentType = ComponentType::HEALTH;
ComponentType TileComponent::componentType = ComponentType::TILE;
ComponentType InventoryComponent::componentType = ComponentType::INVENTORY;
ComponentType CollisionComponent::componentType = ComponentType::COLLISION;
ComponentType SpriteComponent::componentType = ComponentType::SPRITE;
ComponentType ItemComponent::componentType = ComponentType::ITEM;
ComponentType ResourceComponent::componentType = ComponentType::RESOURCE;
ComponentType TableComponent::componentType = ComponentType::TABLE;
ComponentType ChestComponent::componentType = ComponentType::CHEST;
ComponentType PlayerGuiComponent::componentType = ComponentType::PLAYER_GUI;
ComponentType PlayerInputComponent::componentType = ComponentType::PLAYER_INPUT;
ComponentType PlayerAnimationComponent::componentType = ComponentType::PLAYER_ANIMATION;

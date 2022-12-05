
#pragma once
#include "../System.hpp"
#include "../ComponentManager.hpp"
#include "../Components.hpp"

class SpriteSystem : public System {
public:
    void update(ComponentManager& componentManager) {
        for (Entity entity : entities) {
            auto& positionComponent = componentManager.get<PositionComponent>(entity);
            auto& spriteComponent = componentManager.get<SpriteComponent>(entity);
            TextureManager::drawTexture(spriteComponent.spriteSheet, {0,0}, {16,16}, positionComponent.position, 5, true);
        }
    }
};
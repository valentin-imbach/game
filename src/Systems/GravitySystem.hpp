
#pragma once
#include "../System.hpp"
#include "../Components.hpp"

class GravitySystem : public System {
public:
    void update() {
        for (Entity entity : entities) {
            auto& positionComponent = componentManager -> get<PositionComponent>(entity);
            positionComponent.position.y += 1;
        }
    }
};
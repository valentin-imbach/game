
#pragma once
#include "../System.hpp"
#include "../Components.hpp"

class CreatureMovementSystem : public System {
public:
	void update(uint dt) {
		for (Entity entity : entities) {
			Direction direction = componentManager->get<DirectionComponent>(entity).direction;
			float speed = componentManager->get<MovementComponent>(entity).speed;
			PositionComponent& positionComponent = componentManager->get<PositionComponent>(entity);
			CreatureState state = componentManager->get<CreatureStateComponent>(entity).state;
			if (state == CreatureState::WALKING) {
				positionComponent.position += dt * speed * unitVectors[int(direction)] / 1000;
			}
		}
	}
};
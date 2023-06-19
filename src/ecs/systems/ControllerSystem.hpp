
#pragma once
#include "System.hpp"
#include "Window.hpp"
#include "Events.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class ControllerSystem : public System {
public:
	void update(InputState& inputState, bool active, uint ticks) {
		for (Entity entity : entities) {
			CreatureStateComponent& creatureStateComponent = ecs -> getComponent<CreatureStateComponent>(entity);
			DirectionComponent& directionComponent = ecs -> getComponent<DirectionComponent>(entity);

			pair sum(1, 1);
			if (active) {
				if (inputState[InputStateId::MOVE_EAST]) sum.x += 1;
				if (inputState[InputStateId::MOVE_NORTH]) sum.y -= 1;
				if (inputState[InputStateId::MOVE_WEST]) sum.x -= 1;
				if (inputState[InputStateId::MOVE_SOUTH]) sum.y += 1;
			}

			// Direction::value oldDirection = directionComponent.direction;
			if (sum.x == 2 && sum.y == 1) directionComponent.direction = Direction::from_int(1);
			if (sum.x == 2 && sum.y == 0) directionComponent.direction = Direction::from_int(2);
			if (sum.x == 1 && sum.y == 0) directionComponent.direction = Direction::from_int(3);
			if (sum.x == 0 && sum.y == 0) directionComponent.direction = Direction::from_int(4);
			if (sum.x == 0 && sum.y == 1) directionComponent.direction = Direction::from_int(5);
			if (sum.x == 0 && sum.y == 2) directionComponent.direction = Direction::from_int(6);
			if (sum.x == 1 && sum.y == 2) directionComponent.direction = Direction::from_int(7);
			if (sum.x == 2 && sum.y == 2) directionComponent.direction = Direction::from_int(8);

			Direction::value oldFacing = creatureStateComponent.facing;
			if (sum.x == 2) creatureStateComponent.facing = Direction::EAST;
			if (sum.x == 0) creatureStateComponent.facing = Direction::WEST;

			CreatureState::value oldState = creatureStateComponent.state;
			if (sum.x == 1 && sum.y == 1) {
				creatureStateComponent.state = CreatureState::IDLE;
			} else {
				creatureStateComponent.state = CreatureState::WALKING;
			}

			if (creatureStateComponent.facing != oldFacing || creatureStateComponent.state != oldState) {
				creatureStateComponent.lastChange = ticks;
			}
		}
	}
};
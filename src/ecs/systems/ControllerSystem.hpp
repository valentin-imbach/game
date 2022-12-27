
#pragma once
#include "System.hpp"
#include "Window.hpp"
#include "Events.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class ControllerSystem : public System {
public:
	void update(InputState& inputState, bool active) {
		for (Entity entity : entities) {
			CreatureState::value& state = ecs -> getComponent<CreatureStateComponent>(entity).state;
			Direction::value& facing = ecs -> getComponent<CreatureStateComponent>(entity).facing;
			bool& stateChanged = ecs -> getComponent<CreatureStateComponent>(entity).stateChanged;

			Direction::value& direction = ecs -> getComponent<DirectionComponent>(entity).direction;

			pair sum(1, 1);
			if (active) {
				if (inputState[size_t(InputStateId::MOVE_EAST)]) sum.x += 1;
				if (inputState[size_t(InputStateId::MOVE_NORTH)]) sum.y -= 1;
				if (inputState[size_t(InputStateId::MOVE_WEST)]) sum.x -= 1;
				if (inputState[size_t(InputStateId::MOVE_SOUTH)]) sum.y += 1;
			}

			Direction::value oldDirection = direction;
			if (sum.x == 2 && sum.y == 1) direction = Direction::from_int(1);
			if (sum.x == 2 && sum.y == 0) direction = Direction::from_int(2);
			if (sum.x == 1 && sum.y == 0) direction = Direction::from_int(3);
			if (sum.x == 0 && sum.y == 0) direction = Direction::from_int(4);
			if (sum.x == 0 && sum.y == 1) direction = Direction::from_int(5);
			if (sum.x == 0 && sum.y == 2) direction = Direction::from_int(6);
			if (sum.x == 1 && sum.y == 2) direction = Direction::from_int(7);
			if (sum.x == 2 && sum.y == 2) direction = Direction::from_int(8);

			Direction::value oldFacing = facing;
			if (sum.x == 2) facing = Direction::EAST;
			if (sum.x == 0) facing = Direction::WEST;

			CreatureState::value oldState = state;
			if (sum.x == 1 && sum.y == 1) {
				state = CreatureState::IDLE;
			} else {
				state = CreatureState::WALKING;
			}

			stateChanged = (facing != oldFacing || state != oldState);
		}
	}
};
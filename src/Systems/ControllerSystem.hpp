
#pragma once
#include "../System.hpp"
#include "../Window.hpp"
#include "../Events.hpp"

class ControllerSystem : public System {
public:
	void update(InputStates& inputStates) {
		for (Entity entity : entities) {
			CreatureState& state = componentManager->get<CreatureStateComponent>(entity).state;
			Direction& facing = componentManager->get<CreatureStateComponent>(entity).facing;
			bool& stateChanged = componentManager->get<CreatureStateComponent>(entity).stateChanged;

			Direction& direction = componentManager->get<DirectionComponent>(entity).direction;

			pair sum(1, 1);
			if (inputStates[size_t(InputState::MOVE_EAST)]) sum.x += 1;
			if (inputStates[size_t(InputState::MOVE_NORTH)]) sum.y -= 1;
			if (inputStates[size_t(InputState::MOVE_WEST)]) sum.x -= 1;
			if (inputStates[size_t(InputState::MOVE_SOUTH)]) sum.y += 1;

			Direction oldDirection = direction;
			if (sum.x == 2 && sum.y == 1) direction = Direction(0);
			if (sum.x == 2 && sum.y == 0) direction = Direction(1);
			if (sum.x == 1 && sum.y == 0) direction = Direction(2);
			if (sum.x == 0 && sum.y == 0) direction = Direction(3);
			if (sum.x == 0 && sum.y == 1) direction = Direction(4);
			if (sum.x == 0 && sum.y == 2) direction = Direction(5);
			if (sum.x == 1 && sum.y == 2) direction = Direction(6);
			if (sum.x == 2 && sum.y == 2) direction = Direction(7);

			Direction oldFacing = facing;
			if (sum.x == 2) facing = Direction::EAST;
			if (sum.x == 0) facing = Direction::WEST;

			CreatureState oldState = state;
			if (sum.x == 1 && sum.y == 1) {
				state = CreatureState::IDLE;
			} else {
				state = CreatureState::WALKING;
			}

			stateChanged = (facing != oldFacing || state != oldState);
		}
	}
};
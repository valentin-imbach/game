
#pragma once
#include "System.hpp"
#include "Window.hpp"
#include "Events.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class ControllerSystem : public System {

	ROSTER(PLAYER, MOVEMENT, DIRECTION, FACING, POSITION)

	void update(InputState& inputState, Camera camera, bool active, uint ticks) {
		for (Entity entity : entities) {
			MovementComponent& movementComponent = ecs->getComponent<MovementComponent>(entity);
			DirectionComponent& directionComponent = ecs -> getComponent<DirectionComponent>(entity);
			FacingComponent& facingComponent = ecs -> getComponent<FacingComponent>(entity);
			PositionComponent& positionComponent = ecs -> getComponent<PositionComponent>(entity);

			pair sum(1, 1);
			if (active) {
				if (inputState[InputStateId::MOVE_EAST]) sum.x += 1;
				if (inputState[InputStateId::MOVE_NORTH]) sum.y -= 1;
				if (inputState[InputStateId::MOVE_WEST]) sum.x -= 1;
				if (inputState[InputStateId::MOVE_SOUTH]) sum.y += 1;

				vec mpos = camera.worldPosition(Window::instance->mousePosition);
				vec ppos = positionComponent.position;
				facingComponent.facing = vec::normalise(mpos - ppos);
			}

			if (sum.x == 2 && sum.y == 1) directionComponent.direction = Direction::from_int(1);
			if (sum.x == 2 && sum.y == 0) directionComponent.direction = Direction::from_int(2);
			if (sum.x == 1 && sum.y == 0) directionComponent.direction = Direction::from_int(3);
			if (sum.x == 0 && sum.y == 0) directionComponent.direction = Direction::from_int(4);
			if (sum.x == 0 && sum.y == 1) directionComponent.direction = Direction::from_int(5);
			if (sum.x == 0 && sum.y == 2) directionComponent.direction = Direction::from_int(6);
			if (sum.x == 1 && sum.y == 2) directionComponent.direction = Direction::from_int(7);
			if (sum.x == 2 && sum.y == 2) directionComponent.direction = Direction::from_int(8);

			MovementState::value oldState = movementComponent.movementState;
			if (sum.x == 1 && sum.y == 1) {
				movementComponent.movementState = MovementState::IDLE;
			} else {
				movementComponent.movementState = MovementState::WALK;
			}

			if (movementComponent.movementState != oldState) movementComponent.movementStart = ticks;
		}
	}
};
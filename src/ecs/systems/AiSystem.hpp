
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class AiSystem : public System {

	ROSTER(AI)

	void update(uint ticks) {
		for (Entity entity : entities) {
			AiComponent& aiComponent = ecs->getComponent<AiComponent>(entity);
			aiComponent.scores[aiComponent.state] += 5;
			auto maxElementIter = std::max_element(aiComponent.scores.begin(), aiComponent.scores.end());
    		int index = maxElementIter - aiComponent.scores.begin();
			if (aiComponent.scores[index] == 0) index = 0;
			AiState::value newState = AiState::from_int(index);
			if (aiComponent.state != newState) {
				aiComponent.change = true;
				aiComponent.lastChange = ticks;
				aiComponent.state = newState;
				if (ecs->hasComponent<MovementComponent>(entity)) {
					ecs->getComponent<MovementComponent>(entity).movementState = MovementState::IDLE;
				}
			} else {
				aiComponent.change = false;
			}

		}
	}
};
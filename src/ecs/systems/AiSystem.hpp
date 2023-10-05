
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class AiSystem : public System {
public:
	void update(uint ticks) {
		for (Entity entity : entities) {
			AiComponent& aiComponent = ecs->getComponent<AiComponent>(entity);
			auto maxElementIter = std::max_element(aiComponent.scores.begin(), aiComponent.scores.end());
    		int index = maxElementIter - aiComponent.scores.begin();
			if (aiComponent.scores[index] == 0) index = 0;
			AiState::value newState = AiState::from_int(index);
			if (aiComponent.state != newState) {
				aiComponent.change = true;
				aiComponent.lastChange = ticks;
				aiComponent.state = newState;
				// LOG(AiState::to_string(newState));
			} else {
				aiComponent.change = false;
			}

			//LOG(AiState::to_string(aiComponent.state));

			//if (aiComponent.state == AiState::WANDER) wander(entity, ticks, change);
		}
	}

	// void wander(Entity entity, uint ticks, bool change = false) {
	// 	AiWanderComponent& aiWanderComponent = ecs->getComponent<AiWanderComponent>(entity);
	// 	PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
	// 	AiMoveComponent& aiMoveComponent = ecs->getComponent<AiMoveComponent>(entity);
	// 	CreatureStateComponent& creatureStateComponent = ecs->getComponent<CreatureStateComponent>(entity);
	// 	DirectionComponent& directionComponent = ecs->getComponent<DirectionComponent>(entity);

	// 	if (change) aiWanderComponent.position = positionComponent.position;

	// 	vec offset = aiWanderComponent.position - positionComponent.position;
	// 	// if (vec::norm(offset) > 5) {
	// 	// 	aiWanderComponent.direction = vec::normalise(offset);
	// 	// } else {
	// 		// float angle = vec::angle(aiWanderComponent.direction);
	// 		// angle += lerp::flicker(float(ticks)/5000);
	// 		// aiWanderComponent.direction = vec::polar(angle);
	// 	//}
	// 	//LOG(aiWanderComponent.direction);

	// 	aiMoveComponent.move = true;
	// 	for (int dir = 1; dir <= 8; dir++) {
	// 		vec step = Direction::unit[dir];
	// 		float score = 0.5 + vec::dot(step, aiWanderComponent.direction) / 2;
	// 		aiMoveComponent.moveScores[dir] += score * score;
	// 	}
	// }
};
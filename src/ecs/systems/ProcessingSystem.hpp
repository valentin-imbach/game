
#pragma once
#include "Components.hpp"
#include "ECS.hpp"
#include "System.hpp"
#include "utils.hpp"

class ProcessingSystem : public System {
public:
	void update(uint ticks) {
		for (Entity entity : entities) {
			ProcessingComponent& processingComponent = ecs->getComponent<ProcessingComponent>(entity);

			if (processingComponent.end && ticks > processingComponent.end) processingComponent.end = 0;
			
			Entity item = processingComponent.input.item;
			if (!processingComponent.end && item) {
				ItemComponent& itemComponent = ecs->getComponent<ItemComponent>(item);
				uint time = ItemTemplate::templates[itemComponent.itemId].properties[ItemProperty::BURN_TIME] * 10000;
				processingComponent.end = ticks + time;
				itemComponent.count -= 1;
				if (itemComponent.count == 0) {
					ecs->addComponent<DeathComponent>({},item);
					processingComponent.input.item = 0;
				}
			}

			if (processingComponent.end && !processingComponent.processing) {
				processingComponent.processing = true;
			}

			if (!processingComponent.end && processingComponent.processing) {
				processingComponent.processing = false;
			}
		}
	}
};
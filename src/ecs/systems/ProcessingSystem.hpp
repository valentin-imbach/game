
#pragma once
#include "Components.hpp"
#include "ECS.hpp"
#include "System.hpp"
#include "utils.hpp"
#include "EntityFactory.hpp"

class ProcessingSystem : public System {

	ROSTER(PROCESSING, FUEL)

	void update(uint ticks, uint dt) {
		for (Entity entity : entities) {
			ProcessingComponent& processingComponent = ecs->getComponent<ProcessingComponent>(entity);
			FuelComponent& fuelComponent = ecs->getComponent<FuelComponent>(entity);

			uint processTime = 2000;

			Entity item = processingComponent.input.item;
			if (!item) {
				processingComponent.process = 0;
				return;
			}

			ItemComponent& itemComponent = ecs->getComponent<ItemComponent>(item);
			ItemId::value product = processingComponent.map[itemComponent.itemId];
			if (!product) {
				processingComponent.process = 0;
				return;
			}

			Entity out = processingComponent.output.item;
			if (out) {
				ItemComponent& outItemComponent = ecs->getComponent<ItemComponent>(out);
				if (outItemComponent.itemId != product || outItemComponent.count >= MAX_STACK) {
					processingComponent.process = 0;
					return;
				}
			}

			processingComponent.process += fuelComponent.heat * dt / processTime;
			if (processingComponent.process >= 1) {
				itemComponent.count -= 1;
				if (itemComponent.count == 0) {
					ecs->addComponent<DeathComponent>({},item);
					processingComponent.input.item = 0;
				}

				if (out) {
					ItemComponent& outItemComponent = ecs->getComponent<ItemComponent>(out);
					outItemComponent.count += 1;
				} else {
					processingComponent.output.item = EntityFactory::createItem(product, 1);
				}

				processingComponent.process = 0;
			}

		}
	}
};
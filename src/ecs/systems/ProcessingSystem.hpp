
#pragma once
#include "Components.hpp"
#include "ECS.hpp"
#include "System.hpp"
#include "utils.hpp"

class ProcessingSystem : public System {
public:
	LightComponent lightComponent = {true, 3, {255, 0, 0, 255}, 3, 0.2f};
	void update(uint ticks) {
		for (Entity entity : entities) {
			ProcessingComponent& processingComponent = ecs->getComponent<ProcessingComponent>(entity);
			SpriteComponent& spriteComponent = ecs->getComponent<SpriteComponent>(entity);

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
				spriteComponent.spriteStack = processingComponent.processingSprites;
				if (processingComponent.light) ecs->addComponent<LightComponent>(lightComponent, entity);
				if (processingComponent.particle) {
					ParticleComponent particleComponent = {};
					particleComponent.emitters[processingComponent.particle] = {ticks};
					ecs->addComponent<ParticleComponent>(particleComponent, entity);
				}
			}

			if (!processingComponent.end && processingComponent.processing) {
				processingComponent.processing = false;
				spriteComponent.spriteStack = processingComponent.normarSprites;
				if (processingComponent.light) ecs->removeComponent<LightComponent>(entity);
				if (processingComponent.particle) ecs->removeComponent<ParticleComponent>(entity);
			}
		}
	}
};
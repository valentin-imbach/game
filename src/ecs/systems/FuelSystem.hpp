
#pragma once
#include "Components.hpp"
#include "ECS.hpp"
#include "System.hpp"
#include "utils.hpp"

class FuelSystem : public System {

	ROSTER(FUEL, SPRITE)

	LightComponent lightComponent = {true, 3, {255, 0, 0, 255}, 3, 0.2f};
	void update(uint ticks, uint dt) {
		for (Entity entity : entities) {
			FuelComponent& fuelComponent = ecs->getComponent<FuelComponent>(entity);
			SpriteComponent& spriteComponent = ecs->getComponent<SpriteComponent>(entity);

			float heatTime = 10;
			float deltaHeat = (fuelComponent.targetHeat - fuelComponent.heat) * dt / 1000 / heatTime;
			fuelComponent.heat += deltaHeat;

			if (fuelComponent.end && ticks > fuelComponent.end) {
				fuelComponent.start = 0;
				fuelComponent.end = 0;
				fuelComponent.targetHeat = 0;
			}
			
			Entity item = fuelComponent.input.item;
			if (!fuelComponent.end && item) {
				ItemComponent& itemComponent = ecs->getComponent<ItemComponent>(item);
				uint time = ItemTemplate::templates[itemComponent.itemId].properties[ItemProperty::BURN_TIME] * 1000;
				fuelComponent.end = ticks + time;
				fuelComponent.start = ticks;
				float th = float(ItemTemplate::templates[itemComponent.itemId].properties[ItemProperty::HEAT]) / 10;
				fuelComponent.targetHeat = th;
				itemComponent.count -= 1;
				if (itemComponent.count == 0) {
					ecs->addComponent<DeathComponent>({},item);
					fuelComponent.input.item = 0;
				}
			}

			if (fuelComponent.end) {
				uint past = ticks - fuelComponent.start;
				uint total = fuelComponent.end - fuelComponent.start;
				fuelComponent.progress = float(past)/total;
			} else {
				fuelComponent.progress = 0;
			}

			if (fuelComponent.end && !fuelComponent.burning) {
				fuelComponent.burning = true;
				spriteComponent.spriteStack = fuelComponent.hotSprites;
				if (fuelComponent.light) ecs->addComponent<LightComponent>(lightComponent, entity);
				if (fuelComponent.particle) {
					ParticleComponent particleComponent = {};
					particleComponent.emitters[fuelComponent.particle] = {ticks};
					ecs->addComponent<ParticleComponent>(particleComponent, entity);
				}
			}

			if (!fuelComponent.end && fuelComponent.burning) {
				fuelComponent.burning = false;
				spriteComponent.spriteStack = fuelComponent.coldSprites;
				if (fuelComponent.light) ecs->removeComponent<LightComponent>(entity);
				if (fuelComponent.particle) ecs->removeComponent<ParticleComponent>(entity);
			}
		}
	}
};
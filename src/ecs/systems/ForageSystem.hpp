
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "Item.hpp"
#include "utils.hpp"
#include "SoundManager.hpp"

class ForageSystem : public System {
public:
	bool update(vec position, Entity item, uint ticks) {
		for (Entity entity : entities) {
			GridComponent& gridComponent = ecs->getComponent<GridComponent>(entity);
			if (!vec::inside(position, vec(gridComponent.anker) - vec(0.5, 0.5), gridComponent.size, false)) continue;
			ResourceComponent& resourceComponent = ecs->getComponent<ResourceComponent>(entity);
			HealthComponent& healthComponent = ecs->getComponent<HealthComponent>(entity);
			SpriteComponent& spriteComponent = ecs->getComponent<SpriteComponent>(entity);

			if (!resourceComponent.toolId) {
				SoundManager::playSound(resourceComponent.soundId);
				healthComponent.health -= 1;
				return true;
			}

			if (hasItemKind(item, resourceComponent.toolId)) {
				if (getItemProperty(item, ItemProperty::LEVEL) >= resourceComponent.level) {
					int damage = std::min(getItemProperty(item, ItemProperty::EFFICIENCY), int(healthComponent.health));
					healthComponent.health -= damage;
					spriteComponent.effects[SpriteEffectId::SHAKE] = {true, ticks};
					SoundManager::playSound(resourceComponent.soundId);
					return true;
				}
			}
		}
		return false;
	}
};
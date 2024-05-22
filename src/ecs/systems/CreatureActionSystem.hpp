
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "EntityFactory.hpp"
#include "ForageSystem.hpp"

class CreatureActionSystem : public System {
public:
	void update(uint ticks, ForageSystem* forageSystem, EntitySet& updateSet) {
		for (Entity entity : entities) {
			ActionComponent& actionComponent = ecs->getComponent<ActionComponent>(entity);
			if (actionComponent.actionState == ActionState::IDLE) continue;

			if (ticks > actionComponent.trigger && !actionComponent.done) {
				if (actionComponent.actionState == ActionState::ATTACK) {	
					//if (!ecs->hasComponent<PlayerComponent>(entity)) continue;
					//PlayerComponent& playerComponent = ecs->getComponent<PlayerComponent>(entity);
					//Entity item = playerComponent.hotbar.itemContainers[playerComponent.activeSlot][0].item;
					Entity item = actionComponent.item;
					if (item && ecs->hasComponent<MeleeItemComponent>(item)) {
						MeleeItemComponent& damageComponent = ecs->getComponent<MeleeItemComponent>(item);
						PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
						vec force = {0.1f, 0.0f};
						if (vec::dist(actionComponent.position, positionComponent.position) > 0.001f) {
							force = vec::normalise(actionComponent.position - positionComponent.position) / 10;
						}
						EntityFactory::createDamageArea(positionComponent.realmId, actionComponent.position, vec(0.2f, 0.2f), ticks, 1, force, entity);	
					} else if (forageSystem->update(actionComponent.position, item, ticks, updateSet)) {
						//playerComponent.lastAction = ticks;
					}
					//playerComponent.lastAction = ticks;
				}
				actionComponent.done = true;
			}
			if (ticks > actionComponent.end) {
				actionComponent.actionState = ActionState::IDLE;
				actionComponent.position = {0, 0};
				actionComponent.start = 0;
				actionComponent.trigger = 0;
				actionComponent.end = 0;
				actionComponent.done = false;
			}
		}
	}
};
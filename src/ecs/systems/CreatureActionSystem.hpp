
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "EntityFactory.hpp"
#include "ForageSystem.hpp"

class CreatureActionSystem : public System {
public:
	void update(uint ticks, RealmManager& realmManager, ForageSystem* forageSystem, EntitySet& updateSet) {
		for (Entity entity : entities) {
			ActionComponent& actionComponent = ecs->getComponent<ActionComponent>(entity);
			if (actionComponent.actionState == ActionState::IDLE) continue;
			if (!actionComponent.actionEnd || actionComponent.actionEnd > ticks) continue;
			if (actionComponent.actionState == ActionState::ATTACK) {	
				if (!ecs->hasComponent<PlayerComponent>(entity)) continue;
				PlayerComponent& playerComponent = ecs->getComponent<PlayerComponent>(entity);
				Entity item = playerComponent.hotbar.itemContainers[playerComponent.activeSlot][0].item;
				if (!item) continue;
				if (ecs->hasComponent<MeleeItemComponent>(item)) {
					MeleeItemComponent& damageComponent = ecs->getComponent<MeleeItemComponent>(item);
					PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
					vec force = vec::normalise(actionComponent.actionPosition - positionComponent.position) / 10;
					Realm* realm = realmManager.getRealm(positionComponent.realmId);
					EntityFactory::createDamageArea(realm, actionComponent.actionPosition, vec(0.2f, 0.2f), ticks, 1, force, entity);
					playerComponent.lastAction = ticks;
				} else if (forageSystem->update(actionComponent.actionPosition, item, ticks, updateSet)) {
					playerComponent.lastAction = ticks;
				}
			}
			actionComponent.actionState = ActionState::IDLE;
			actionComponent.actionPosition = {};
			actionComponent.actionStart = 0;
			actionComponent.actionEnd = 0;
		}
	}
};
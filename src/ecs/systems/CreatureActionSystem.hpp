
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
			CreatureStateComponent& creatureStateComponent = ecs->getComponent<CreatureStateComponent>(entity);
			if (creatureStateComponent.actionState == ActionState::IDLE) continue;
			if (!creatureStateComponent.actionEnd || creatureStateComponent.actionEnd > ticks) continue;
			if (creatureStateComponent.actionState == ActionState::ATTACK) {	
				if (!ecs->hasComponent<PlayerComponent>(entity)) continue;
				PlayerComponent& playerComponent = ecs->getComponent<PlayerComponent>(entity);
				Entity item = playerComponent.hotbar.itemContainers[playerComponent.activeSlot][0].item;
				if (!item) continue;
				if (ecs->hasComponent<MeleeItemComponent>(item)) {
					MeleeItemComponent& damageComponent = ecs->getComponent<MeleeItemComponent>(item);
					PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
					vec force = vec::normalise(creatureStateComponent.actionPosition - positionComponent.position) / 10;
					Realm* realm = realmManager.getRealm(positionComponent.realmId);
					EntityFactory::createDamageArea(realm, creatureStateComponent.actionPosition, vec(0.2f, 0.2f), ticks, 1, force, entity);
					playerComponent.lastAction = ticks;
				} else if (forageSystem->update(creatureStateComponent.actionPosition, item, ticks, updateSet)) {
					playerComponent.lastAction = ticks;
				}
			}
			creatureStateComponent.actionState = ActionState::IDLE;
			creatureStateComponent.actionPosition = {};
			creatureStateComponent.actionStart = 0;
			creatureStateComponent.actionEnd = 0;
		}
	}
};
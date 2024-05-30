
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

			if (actionComponent.actionState == ActionState::ATTACK) {
				if (actionComponent.trigger && ticks > actionComponent.trigger) {
					Entity item = actionComponent.item;
					if (item && ecs->hasComponent<MeleeItemComponent>(item)) {
						MeleeItemComponent& damageComponent = ecs->getComponent<MeleeItemComponent>(item);
						PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
						vec force = {0.1f, 0.0f};
						if (vec::dist(actionComponent.position, positionComponent.position) > 0.001f) {
							force = vec::normalise(actionComponent.position - positionComponent.position) / 10;
						}
						EntityFactory::createDamageArea(positionComponent.realmId, actionComponent.position, vec(0.2f, 0.2f), ticks, 1, force, entity);	
					} else {
						forageSystem->update(actionComponent.position, item, ticks, updateSet);
					}
					actionComponent.trigger = 0;
				}

				if (ticks > actionComponent.end) {
					actionComponent.actionState = ActionState::IDLE;
					actionComponent.position = {0, 0};
					actionComponent.start = 0;
					actionComponent.trigger = 0;
					actionComponent.end = 0;
				}
			}

			if (actionComponent.actionState == ActionState::CHARGE) {
				Entity item = actionComponent.item;
				if (!item || !ecs->hasComponent<LauncherComponent>(item)) return;
				LauncherComponent& launcherComponent = ecs->getComponent<LauncherComponent>(item);

				uint past = ticks - actionComponent.start;
				float t = float(past) / launcherComponent.chargeTime;
				float charge = launcherComponent.maxForce * std::min(t, 1.0f);

				if (actionComponent.trigger && ticks > actionComponent.trigger) {				
					if (charge > launcherComponent.minForce) {
						PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
						vec target = actionComponent.position;

						vec direction = {1.0f, 0};
						if (vec::dist(target, positionComponent.position) > 0.001f) {
							direction = vec::normalise(target - positionComponent.position);
						}

						EntityFactory::createProjectile(positionComponent.realmId, positionComponent.position, charge * direction, entity);
					}

					actionComponent.actionState = ActionState::IDLE;
					actionComponent.position = {0, 0};
					actionComponent.start = 0;
					actionComponent.trigger = 0;
					actionComponent.end = 0;

					ecs->getComponent<SpriteComponent>(item).spriteStack.setSprite(0, Sprite(SpriteSheet::TOOLS, pair(3, 4)));
					ecs->getComponent<SpriteComponent>(item).spriteStack.setSprite(1, Sprite(SpriteSheet::TOOLS, pair(3, 5)));
				} else {
					if (past > launcherComponent.chargeTime) {
						ecs->getComponent<SpriteComponent>(item).spriteStack.setSprite(0, Sprite(SpriteSheet::TOOLS, pair(5, 4)));
						ecs->getComponent<SpriteComponent>(item).spriteStack.setSprite(1, Sprite(SpriteSheet::TOOLS, pair(5, 5)));
					} else if (charge > launcherComponent.minForce) {
						ecs->getComponent<SpriteComponent>(item).spriteStack.setSprite(0, Sprite(SpriteSheet::TOOLS, pair(4, 4)));
						ecs->getComponent<SpriteComponent>(item).spriteStack.setSprite(1, Sprite(SpriteSheet::TOOLS, pair(4, 5)));
					}
				}
			}


		}
	}
};
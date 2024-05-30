
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class PlayerSystem : public System {
public:
	void update() {
		for (Entity entity : entities) {
			HandComponent& handComonent = ecs->getComponent<HandComponent>(entity);
			PlayerComponent& playerComponent = ecs->getComponent<PlayerComponent>(entity);

			uchar sel = playerComponent.activeSlot;
			Entity item = playerComponent.hotbar.itemContainers[sel]->item;
			handComonent.item = item;
		}
	}
};
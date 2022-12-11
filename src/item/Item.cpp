
#include "Item.hpp"
#include "Components.hpp"
#include "ECS.hpp"

void Item::draw(pair position, int scale, ECS* ecs) {
		if (entity) {
			SpriteStack& spriteStack = ecs->getComponent<SpriteComponent>(entity).spriteStack;
			spriteStack.draw(position, scale);
		}
	}
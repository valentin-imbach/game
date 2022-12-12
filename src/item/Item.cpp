
#include "Item.hpp"
#include <string>
#include "Components.hpp"
#include "ECS.hpp"
#include "Sprite.hpp"
#include "TextManager.hpp"

void Item::draw(pair position, int scale, ECS* ecs) {
	if (entity) {
		SpriteStack& spriteStack = ecs->getComponent<SpriteComponent>(entity).spriteStack;
		spriteStack.draw(position, scale);
	} else if (itemId != ItemId::NONE) {
		int index = int(itemId) - 1;
		pair pos = {index % 6, index / 6};
		Sprite sprite = Sprite(SpriteSheet::ITEMS, pos, {1, 1});
		sprite.draw(position, scale);
		std::string text = std::to_string(count);
		TextManager::drawText(text, position);
	}
}
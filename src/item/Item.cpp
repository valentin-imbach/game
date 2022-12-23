
#include "Item.hpp"
#include <string>
#include "Components.hpp"
#include "ECS.hpp"
#include "Sprite.hpp"
#include "TextManager.hpp"

Item::Item(Entity entity) : entity(entity), count(1) {}

Item::Item(ItemId::value itemId, int count) : itemId(itemId), count(count) {
	assert(0 < count && count <= MAX_STACK);
}

Item::operator bool() {
	return (entity != 0 || itemId != ItemId::NONE);
}

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

[[nodiscard]] Item ItemContainer::add(Item other, ItemAmount::value amount) {
	int number = MAX_STACK - item.count;
	if (amount == ItemAmount::ALL) number = std::min(number, other.count);
	if (amount == ItemAmount::HALF) number = std::min(number, (other.count + 1) / 2);
	if (amount == ItemAmount::ONE) number = std::min(number, 1);
	if (other.entity) {
		if (!item) {
			item = other;
			other = Item();
		}
	} else if (other.itemId == item.itemId || !item) {
		item.itemId = other.itemId;
		item.count += number;
		other.count -= number;
		assert(other.count >= 0);
		if (other.count == 0) {
			other.itemId = ItemId::NONE;
		}
	}
	return other;
}

void ItemContainer::clear() {
	item = Item();
	//TODO delete Entity
}

Inventory::Inventory(pair size) : size(size) {
	itemContainers = std::vector<std::vector<ItemContainer>>(size.x, std::vector<ItemContainer>(size.y));
}

Item Inventory::add(Item item) {
	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {
			item = itemContainers[x][y].add(item);
		}
	}
	return item;
}

void Inventory::clear() {
	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {
			itemContainers[x][y].clear();
		}
	}
}
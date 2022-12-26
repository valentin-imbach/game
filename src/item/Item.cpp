
#include "Item.hpp"
#include <string>
#include "Components.hpp"
#include "ECS.hpp"
#include "Sprite.hpp"
#include "TextManager.hpp"
#include "EntityFactory.hpp"

[[nodiscard]] Entity ItemContainer::add(Entity other, ItemAmount::value amount) {
	if (!other) return other;
	ItemComponent& otherComponent = EntityFactory::ecs->getComponent<ItemComponent>(other);

	if (item) {
		ItemComponent& itemComponent = EntityFactory::ecs->getComponent<ItemComponent>(item);
		if (itemComponent.itemId == ItemId::NONE) return other;
		if (itemComponent.itemId != otherComponent.itemId) return other;
		uint addCount = MAX_STACK - itemComponent.count;
		if (amount == ItemAmount::ALL) addCount = std::min(addCount, otherComponent.count);
		if (amount == ItemAmount::HALF) addCount = std::min(addCount, (otherComponent.count + 1) / 2);
		if (amount == ItemAmount::ONE) addCount = std::min(addCount, 1u);
		otherComponent.count -= addCount;
		itemComponent.count += addCount;
		if (!otherComponent.count) {
			EntityFactory::ecs->destroyEntity(other);
			return 0;
		}
	} else {
		uint addCount = MAX_STACK;
		if (amount == ItemAmount::ALL) addCount = std::min(addCount, otherComponent.count);
		if (amount == ItemAmount::HALF) addCount = std::min(addCount, (otherComponent.count + 1) / 2);
		if (amount == ItemAmount::ONE) addCount = std::min(addCount, 1u);

		if (otherComponent.count == addCount) {
			item = other;
			return 0;
		} else {
			otherComponent.count -= addCount;
			item = EntityFactory::createItem(otherComponent.itemId, addCount);
		}		
	}
	return other;
}

void ItemContainer::clear() {
	item = 0;
}

void ItemContainer::draw(pair position, uint scale) {
	if (!item) return;
	ItemComponent& itemComponent = EntityFactory::ecs->getComponent<ItemComponent>(item);
	SpriteStack& spriteStack = EntityFactory::ecs->getComponent<SpriteComponent>(item).spriteStack;
	spriteStack.draw(position, scale);
	if (itemComponent.count == 1) return;
	std::string text = std::to_string(itemComponent.count);
	TextManager::drawText(text, position);
}

Inventory::Inventory(pair size) : size(size) {
	itemContainers = std::vector<std::vector<ItemContainer>>(size.x, std::vector<ItemContainer>(size.y));
}

Entity Inventory::add(Entity item) {
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
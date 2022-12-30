
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
			EntityFactory::ecs->dead.push_back(other);
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

void ItemContainer::drawInfo(pair position, bool elaborate) {
	if (!item) return;
	ItemComponent& itemComponent = EntityFactory::ecs->getComponent<ItemComponent>(item);
	if (!itemComponent.itemId) return;
	ItemTemplate* itemTemplate = ItemTemplate::templates[itemComponent.itemId].get();

	// std::vector<Text> texts;
	//  v(int) h;

	int spacing = 30;
	int width = 0;

	std::vector<std::pair<Text, pair>> texts;
	std::vector<std::pair<pair, pair>> icons;
	pair pos = position;

	if (elaborate) {
		texts.emplace_back(Text(itemTemplate->name, TTF_STYLE_UNDERLINE), pos);
		pos.y += spacing;
		for (int kind = 1; kind <= ItemKind::count; kind++) {
			if (!itemTemplate->kinds[kind]) continue;
			icons.emplace_back(pair((kind - 1) % 8, (kind - 1) / 8), pos);
			ItemKindTemplate* itemKindTemplate = ItemKindTemplate::templates[kind].get();
			std::string kindText = "   " + itemKindTemplate->name;
			texts.emplace_back(Text(kindText, TTF_STYLE_BOLD), pos);
			pos.y += spacing;
			for (int property = 1; property <= ItemProperty::count; property++) {
				if (!itemKindTemplate->properties[property]) continue;
				ItemPropertyTemplate* itempPropertyTemplate = ItemPropertyTemplate::templates[property].get();
				int value = itemTemplate->properties[ItemProperty::from_int(property)];
				std::string propertyText = itempPropertyTemplate->name + ": " + std::to_string(value);
				texts.emplace_back(Text(propertyText, TTF_STYLE_NORMAL), pos);
				pos.y += spacing;
			}
		}
	} else {
		texts.emplace_back(Text(itemTemplate->name, TTF_STYLE_NORMAL), pos);
	}

	pair size = {0, texts.size() * spacing};
	for (auto& text : texts) size.x = std::max(size.x, TextManager::textSize(text.first.text).x);
	TextureManager::drawRect({position.x - 10, position.y}, {size.x + 20, size.y}, {0, 0, 0, 150}, false, true);

	for (auto& text : texts) {
		TextManager::drawText(text.first, text.second);
	}

	for (auto& icon : icons) {
		Sprite sprite = Sprite(SpriteSheet::ICONS_WHITE, icon.first);
		sprite.draw(icon.second, 2, false);
	}
}

//* Inventory

Inventory::Inventory(pair size) : size(size) {
	assert(0 <= size.x && size.x <= INV_WIDTH);
	assert(0 <= size.y && size.y <= INV_HEIGHT);
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
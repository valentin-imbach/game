
#include "Item.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "Sprite.hpp"
#include "TextManager.hpp"
#include "EntityFactory.hpp"
#include "World.hpp"

[[nodiscard]] Entity ItemContainer::add(Entity other, ItemAmount::value amount) {
	if (!other) return other;
	if (itemKind && !hasItemKind(other, itemKind)) return other;
	if (output) return other;

	ItemComponent& otherComponent = EntityFactory::world->ecs.getComponent<ItemComponent>(other);
	if (item) {
		ItemComponent& itemComponent = EntityFactory::world->ecs.getComponent<ItemComponent>(item);
		if (itemComponent.itemId == ItemId::NONE) return other;
		if (itemComponent.itemId != otherComponent.itemId) return other;
		uint addCount = MAX_STACK - itemComponent.count;
		if (amount == ItemAmount::ALL) addCount = std::min(addCount, otherComponent.count);
		if (amount == ItemAmount::HALF) addCount = std::min(addCount, (otherComponent.count + 1) / 2);
		if (amount == ItemAmount::ONE) addCount = std::min(addCount, 1u);
		otherComponent.count -= addCount;
		itemComponent.count += addCount;
		if (!otherComponent.count) {
			EntityFactory::world->ecs.addComponent(DeathComponent(), other);
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

void ItemContainer::clear(bool destroy) {
	if (destroy) EntityFactory::world->ecs.addComponent(DeathComponent(), item);
	item = 0;
}

void ItemContainer::draw(vec position, uint scale) {
	if (!item) {
		if (itemKind) {
			pair source((itemKind - 1) % 8, (itemKind - 1) / 8);
			Sprite sprite = Sprite(SpriteSheet::ICONS_BLACK, source);
			sprite.draw(position, scale);
		} else if (output) {
			Sprite sprite = Sprite(SpriteSheet::OUTPUT);
			sprite.draw(position, scale);
		}
		return;
	}
	ItemComponent& itemComponent = EntityFactory::world->ecs.getComponent<ItemComponent>(item);
	SpriteStack& spriteStack = EntityFactory::world->ecs.getComponent<SpriteComponent>(item).spriteStack;
	spriteStack.draw(position, scale);
	if (itemComponent.count == 1) return;
	std::string text = std::to_string(itemComponent.count);
	TextManager::drawText(text, position);
}

void ItemContainer::drawInfo(vec position, bool elaborate) {
	if (!item) return;
	ItemComponent& itemComponent = EntityFactory::world->ecs.getComponent<ItemComponent>(item);

	int spacing = 30;

	std::vector<std::pair<Text, vec>> texts;
	std::vector<std::pair<pair, vec>> icons;
	vec pos = position;

	if (!itemComponent.itemId) {
		std::string name = "No Name";
		if (EntityFactory::world->ecs.hasComponent<NameComponent>(item)) {
			name = EntityFactory::world->ecs.getComponent<NameComponent>(item).name.get();	
		}
		if (elaborate) {
			texts.emplace_back(Text(name, TTF_STYLE_UNDERLINE), pos);
			pos.y += spacing;

			if (EntityFactory::world->ecs.hasComponent<DurabilityComponent>(item)) {
				DurabilityComponent& durabilityComponent = EntityFactory::world->ecs.getComponent<DurabilityComponent>(item);
				std::string text = "Durability: " + std::to_string(durabilityComponent.durability) + "/" +  std::to_string(durabilityComponent.maxDurability);
				texts.emplace_back(Text(text), pos);
				pos.y += spacing;
			}
			if (EntityFactory::world->ecs.hasComponent<ItemModComponent>(item)) {
				ItemModComponent& itemModComponent = EntityFactory::world->ecs.getComponent<ItemModComponent>(item);
				for (int i = 1; i < ItemModId::count; i++) {
					int val = itemModComponent.mods[i];
					if (val == 0) continue;
					std::string sign = (val > 0) ? "+" : "";
					std::string text = "Movement Speed: " + sign + std::to_string(val) + "%";
					texts.emplace_back(Text(text), pos);
					pos.y += spacing;
				}
			}
			if (EntityFactory::world->ecs.hasComponent<MeleeItemComponent>(item)) {
				MeleeItemComponent& damageComponent = EntityFactory::world->ecs.getComponent<MeleeItemComponent>(item);
				texts.emplace_back(Text("  Weapon", TTF_STYLE_BOLD), pos);
				pos.y += spacing;
				std::string text = "Damage: " + std::to_string(damageComponent.damage);
				texts.emplace_back(Text(text), pos);
				pos.y += spacing;
			}
			if (EntityFactory::world->ecs.hasComponent<ItemKindComponent>(item)) {
				ItemKindComponent& itemKindComponent = EntityFactory::world->ecs.getComponent<ItemKindComponent>(item);
				for (uint kind = 1; kind <= ItemKind::count; kind++) {
					if (!itemKindComponent.itemKinds[kind]) continue;
					icons.emplace_back(pair((kind - 1) % 8, (kind - 1) / 8), pos);
					ItemKindTemplate& itemKindTemplate = ItemKindTemplate::templates[kind];
					std::string kindText = "   " + itemKindTemplate.name;
					texts.emplace_back(Text(kindText, TTF_STYLE_BOLD), pos);
					pos.y += spacing;
					for (uint property = 1; property <= ItemProperty::count; property++) {
						if (!itemKindTemplate.properties[property]) continue;
						ItemPropertyTemplate& itempPropertyTemplate = ItemPropertyTemplate::templates[property];
						int value = itemKindComponent.itemProperties[property];
						std::string propertyText = itempPropertyTemplate.name + ": " + std::to_string(value);
						texts.emplace_back(Text(propertyText, TTF_STYLE_NORMAL), pos);
						pos.y += spacing;
					}
				}
			}
		} else {
			texts.emplace_back(Text(name), pos);
			pos.y += spacing;
		}
		
	} else {
		ItemTemplate& itemTemplate = ItemTemplate::templates[itemComponent.itemId];
		if (elaborate) {
			texts.emplace_back(Text(itemTemplate.name, TTF_STYLE_UNDERLINE), pos);
			pos.y += spacing;
			for (uint kind = 1; kind <= ItemKind::count; kind++) {
				if (!itemTemplate.kinds[kind]) continue;
				icons.emplace_back(pair((kind - 1) % 8, (kind - 1) / 8), pos);
				ItemKindTemplate& itemKindTemplate = ItemKindTemplate::templates[kind];
				std::string kindText = "   " + itemKindTemplate.name;
				texts.emplace_back(Text(kindText, TTF_STYLE_BOLD), pos);
				pos.y += spacing;
				for (uint property = 1; property <= ItemProperty::count; property++) {
					if (!itemKindTemplate.properties[property]) continue;
					ItemPropertyTemplate& itempPropertyTemplate = ItemPropertyTemplate::templates[property];
					int value = itemTemplate.properties[ItemProperty::from_int(property)];
					std::string propertyText = itempPropertyTemplate.name + ": " + std::to_string(value);
					texts.emplace_back(Text(propertyText, TTF_STYLE_NORMAL), pos);
					pos.y += spacing;
				}
			}
		} else {
			texts.emplace_back(Text(itemTemplate.name, TTF_STYLE_NORMAL), pos);
		}
	}

	pair size(0, texts.size() * spacing);
	for (auto& text : texts) size.x = std::max(size.x, TextManager::getTextSize(text.first.text).x);
	TextureManager::drawRect({position.x - 10, position.y}, vec(size.x + 20, size.y), {0, 0, 0, 150}, false, true);

	for (auto& text : texts) {
		TextManager::drawText(text.first, text.second);
	}

	for (auto& icon : icons) {
		Sprite sprite = Sprite(SpriteSheet::ICONS_WHITE, icon.first);
		TextureStyle style;
		style.centered = false;
		sprite.draw(icon.second, 2, style);
	}
}

//* Inventory

Inventory::Inventory(pair size) : size(size) {
	assert(0 <= size.x && size.x <= INV_WIDTH);
	assert(0 <= size.y && size.y <= INV_HEIGHT);
}

Entity Inventory::add(Entity item, pair range) {
	int y1 = range.x;
	int y2 = std::min(size.y, range.y);
	for (int y = y1; y < y2; y++) {
		for (int x = 0; x < size.x; x++) {
			item = itemContainers[x][y].add(item);
		}
	}
	return item;
}

int Inventory::count() {
	int counter = 0;
	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {
			if (itemContainers[x][y].item) counter += 1;
		}
	}
	return counter;
}

void Inventory::clear(bool destroy) {
	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {
			itemContainers[x][y].clear(destroy);
		}
	}
}

bool hasItemKind(Entity item, ItemKind::value itemKind) {
	if (EntityFactory::world->ecs.hasComponent<ItemKindComponent>(item)) {
		return EntityFactory::world->ecs.getComponent<ItemKindComponent>(item).itemKinds[itemKind];
	} else if (EntityFactory::world->ecs.hasComponent<ItemComponent>(item)) {
		ItemComponent& itemComponent = EntityFactory::world->ecs.getComponent<ItemComponent>(item);
		if (!itemComponent.itemId) return false;
		return ItemTemplate::templates[itemComponent.itemId].kinds[itemKind];
	}
	return false;
}

int getItemProperty(Entity item, ItemProperty::value itemProperty) {
	if (EntityFactory::world->ecs.hasComponent<ItemKindComponent>(item)) {
		return EntityFactory::world->ecs.getComponent<ItemKindComponent>(item).itemProperties[itemProperty];
	} else if (EntityFactory::world->ecs.hasComponent<ItemComponent>(item)) {
		ItemComponent& itemComponent = EntityFactory::world->ecs.getComponent<ItemComponent>(item);
		if (!itemComponent.itemId) return 0;
		return ItemTemplate::templates[itemComponent.itemId].properties[itemProperty];
	}
	return 0;
}
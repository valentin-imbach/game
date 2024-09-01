
#include "Loot.hpp"
#include "EntityFactory.hpp"

Entity Loot::create(uint seed) {
	int count = noise::Int(seed, range.x, range.y);
	if (count <= 0 || !itemId) return 0;
	return EntityFactory::createItem(itemId, count);
}

Entity Loot::create(uint seed, RealmId realmId, vec position) {
	int count = noise::Int(seed, range.x, range.y);
	if (count <= 0 || !itemId) return 0;
	return EntityFactory::createItem(itemId, count, realmId, position);
}

Entity Loot::fromValue(uint seed, uint value) {
	std::vector<ItemId::value> itemIDs;
	for (int i = 1; i < ItemId::count; i++) {
		if (ItemTemplate::templates[i].value <= value) itemIDs.push_back(ItemId::from_int(i));
	}
	if (itemIDs.empty()) return 0;
	ItemId::value itemId = noise::choice(seed, itemIDs);
	int count = value / ItemTemplate::templates[itemId].value;
	return EntityFactory::createItem(itemId, count);
}
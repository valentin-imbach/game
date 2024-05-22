
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
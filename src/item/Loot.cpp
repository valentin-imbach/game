
#include "Loot.hpp"
#include "EntityFactory.hpp"

Entity Loot::create(uint seed) {
	int count = noise::Int(seed, range.x, range.y);
	if (count <= 0 || !itemId) return 0;
	return EntityFactory::createItem(itemId, count);
}

Entity Loot::create(uint seed, Realm* realm, vec position) {
	int count = noise::Int(seed, range.x, range.y);
	if (count <= 0 || !itemId) return 0;
	return EntityFactory::createItem(itemId, count, realm, position);
}
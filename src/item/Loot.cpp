
#include "Loot.hpp"
#include "EntityFactory.hpp"

Entity Loot::create(uint seed) {
	int count = rand_int(seed, range.x, range.y);
	if (count <= 0 || !itemId) return 0;
	return EntityFactory::createItem(itemId, count);
}

Entity Loot::create(uint seed, vec position) {
	int count = rand_int(seed, range.x, range.y);
	if (count <= 0 || !itemId) return 0;
	return EntityFactory::createItem(itemId, count, position);
}
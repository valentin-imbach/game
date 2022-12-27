
#pragma once
#include "utils.hpp"
#include "EntityFactory.hpp"

struct Loot {
	ItemId::value itemId;
	pair range;
	float chance = 1.0f;

	Entity create(uint seed) {
		int count = rand_int(seed, range.x, range.y);
		if (count <= 0 || !itemId) return 0;
		return EntityFactory::createItem(itemId, count);
	}

	Entity create(uint seed, vec position) {
		int count = rand_int(seed, range.x, range.y);
		if (count <= 0 || !itemId) return 0;
		return EntityFactory::createItem(itemId, count, position);
	}
};
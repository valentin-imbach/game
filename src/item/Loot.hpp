
#pragma once
#include "utils.hpp"
#include "ECS_types.hpp"
#include "Item_types.hpp"

#define LOOT_SIZE 3
class Realm;

struct Loot {
	ItemId::value itemId;
	pair range;
	float chance = 1.0f;

	Entity create(uint seed);
	Entity create(uint seed, Realm* realm, vec position);
};

struct LootTable {
	uint size = 0;
	std::array<Loot, LOOT_SIZE> table;

	void addLoot(Loot loot) {
		assert(size < LOOT_SIZE);
		table[size] = loot;
		size += 1;
	}
};
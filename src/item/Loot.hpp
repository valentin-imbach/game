
#pragma once
#include "utils.hpp"
#include "ECS_types.hpp"
#include "Item_types.hpp"
#include "ItemTemplates.hpp"

#define LOOT_SIZE 5

struct Loot {
	ItemId::value itemId;
	pair range;
	float chance = 1.0f;

	Entity create(uint seed);
	Entity create(uint seed, RealmId realmId, vec position);

	static Entity fromValue(uint seed, uint value);
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

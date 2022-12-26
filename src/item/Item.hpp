
#pragma once
#include <vector>
#include "utils.hpp"
#include "ECS_types.hpp"
#include "Item_types.hpp"

struct ItemContainer {
	Entity item;
	[[nodiscard]] Entity add(Entity other, ItemAmount::value amount = ItemAmount::ALL);
	void draw(pair position, uint scale);
	void clear();
};

class Inventory {
public:
	pair size;
	Inventory(pair size = {0, 0});
	
	std::vector<std::vector<ItemContainer>> itemContainers;

	[[nodiscard]] Entity add(Entity item);

	void clear();
};
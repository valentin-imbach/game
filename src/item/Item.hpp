
#pragma once
#include <vector>
#include "utils.hpp"
#include "ECS_types.hpp"
#include "Item_types.hpp"

#define INV_WIDTH 7
#define INV_HEIGHT 5

struct ItemContainer {
	Entity item = 0;
	ItemKind::value itemKind = ItemKind::NONE;

	[[nodiscard]] Entity add(Entity other, ItemAmount::value amount = ItemAmount::ALL);
	void draw(pair position, uint scale);
	void drawInfo(pair position, bool elaborate);
	void clear(bool destroy = false);
};


class Inventory {
public:
	pair size;
	Inventory(pair size = {0, 0});
	
	ItemContainer itemContainers[INV_WIDTH][INV_HEIGHT];

	[[nodiscard]] Entity add(Entity item);

	void clear(bool destroy = false);
};
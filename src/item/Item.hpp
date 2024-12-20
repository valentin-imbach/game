
#pragma once
#include "utils.hpp"
#include "ECS_types.hpp"
#include "Item_types.hpp"

#define INV_WIDTH 7
#define INV_HEIGHT 6

struct ItemContainer {
	Entity item = 0;
	ItemKind::value itemKind = ItemKind::NONE;
	bool output = false;

	[[nodiscard]] Entity add(Entity other, ItemAmount::value amount = ItemAmount::ALL);
	void draw(vec position, uint scale);
	void drawInfo(vec position, bool elaborate);
	void clear(bool destroy = false);
};

class Inventory {
public:
	pair size;
	Inventory(pair size = {0, 0});
	
	ItemContainer itemContainers[INV_WIDTH][INV_HEIGHT];

	[[nodiscard]] Entity add(Entity item, pair range = pair(0, INV_HEIGHT));

	int count();
	void clear(bool destroy = false);
};

bool hasItemKind(Entity item, ItemKind::value itemKind);
int getItemProperty(Entity item, ItemProperty::value itemProperty);

#pragma once
#include "utils.hpp"
#include "Item_types.hpp"

using ItemPropertyBitset = std::bitset<ItemProperty::count>;
using ItemKindBitset = std::bitset<ItemKind::count>;
using ItemProperties = std::unordered_map<ItemProperty::value, uchar>;

struct ItemPropertyTemplate {
	std::string name = "?";

	static std::array<ItemPropertyTemplate, ItemProperty::count> templates;
	static void setTemplates();
};

struct ItemKindTemplate {
	std::string name = "?";
	ItemPropertyBitset properties;

	static std::array<ItemKindTemplate, ItemKind::count> templates;
	static void setTemplates();
};

struct ItemTemplate {
	std::string name = "?";
	ItemKindBitset kinds;
	std::unordered_map<ItemProperty::value, int> properties;
	uint value = 1;

	static std::array<ItemTemplate, ItemId::count> templates;
	static void setTemplates();
};

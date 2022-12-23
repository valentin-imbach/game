
#pragma once
#include "utils.hpp"
#include "Item_types.hpp"

using ItemPropertyBitset = std::bitset<ItemProperty::count>;
using ItemKindBitset = std::bitset<ItemKind::count>;
using ItemProperties = std::unordered_map<ItemProperty::value, uint8_t>;

struct ItemPropertyTemplate {
    std::string name;
    ItemPropertyTemplate(std::string name) : name(name) {}

    static std::array<std::unique_ptr<ItemPropertyTemplate>, ItemProperty::count + 1> templates;
	static void setTemplates();
};

struct ItemKindTemplate {
    std::string name;
    ItemPropertyBitset properties;
    ItemKindTemplate(std::string name, ItemPropertyBitset properties) : name(name), properties(properties) {}

    static std::array<std::unique_ptr<ItemKindTemplate>, ItemKind::count + 1> templates;
	static void setTemplates();
};

struct ItemTemplate {
    std::string name;
    ItemKindBitset kinds;
    std::unordered_map<ItemProperty::value, int> properties;
    ItemTemplate(std::string name, ItemKindBitset kinds) : name(name), kinds(kinds) {}

    static std::array<std::unique_ptr<ItemTemplate>, ItemId::count + 1> templates;
    static void setTemplates();
};

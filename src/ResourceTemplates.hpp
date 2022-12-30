
#pragma once
#include "utils.hpp"
#include "Item_types.hpp"
#include "Loot.hpp"

ENUM(ResourceId,
BASALT_ROCK,
OBSIDIAN_ROCK,
GRANITE_ROCK,
SANDSTONE_ROCK,
CHALK_ROCK,

PINE_TREE,
OAK_TREE,
ROSEWOOD_TREE,
ASHWOOD_TREE,
SPRUCE_TREE,

BUSH)

struct ResourceTemplate {
    
    pair size;

    pair position;
    uint8_t height;
    uint8_t variations = 1;

    ToolId::value toolId;
    uint8_t level;

    //SoundID sound;

    LootTable lootTable;

    ResourceTemplate(pair size, pair position, int height, ToolId::value toolId, int level) : size(size), position(position), height(height), toolId(toolId), level(level) {}

    static std::array<std::unique_ptr<ResourceTemplate>, ResourceId::count> templates;
    static void setTemplates();
};

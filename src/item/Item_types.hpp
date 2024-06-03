
#pragma once
#include "enum.hpp"

#define MAX_STACK 99u

ENUM(ItemKind,

//* Tools
PICK_AXE,
AXE,
SHOVEL,
HAMMER,
SCYTHE,
HOE,
KNIFE,
ACCESSORY,

//* Weapons
SWORD,
BOW,
ARROW,
SHIELD,

//* Materials
COBBLE,
ORE,
PEBBLE,
GEM,
LOG,
PLANK,
ROD,
LEATHER,
FOOD,
ORGANIC,
DUST,
ROPE,

//* Crafting
INGOT,
PLATE,
GLASS,
PAPER,
GEAR,
FABRIC,
FUEL,
POTION,

//* Clothing
CLOTHING_HEAD,
CLOTHING_BODY,
CLOTHING_LEGS,
CLOTHING_FEET,

//* Armor
ARMOR_HEAD,
ARMOR_BODY,
ARMOR_LEGS,
ARMOR_FEET)

ENUM(ItemProperty,
//* Mechanical
WEIGHT,
STRENGTH,
HARDNESS,
FLEXIBILITY,
TOUGHNESS,

//* Metal
MELTING_POINT,

//* Food
NUTRITION,

//* Optical
REFLECTIVITY,
LUMINOCITY,
OPACITY,

//* Fuel
BURN_TIME,
HEAT,

//* Tools
EFFICIENCY,
DURABILITY,
PRECISION,
EFFECTIVENESS,
LEVEL,

//* Weapons
DAMAGE,
PARRY,

//* Clothing
VANITY,
COMFORT,
WARMTH)

ENUM(ItemId,

OAK_LOG,
OAK_BRANCH,
OAK_PLANK,
OAK_STICK,
OAK_GEAR,

MAPLE_LOG,
MAPLE_BRANCH,
MAPLE_PLANK,
MAPLE_STICK,
MAPLE_GEAR,

SPRUCE_LOG,
SPRUCE_BRANCH,
SPRUCE_PLANK,
SPRUCE_STICK,
SPRUCE_GEAR,

BIRCH_LOG,
BIRCH_BRANCH,
BIRCH_PLANK,
BIRCH_STICK,
BIRCH_GEAR,

BASALT_COBBLE,
OBSIDIAN_COBBLE,
GRANITE_COBBLE,
SANDSTONE_COBBLE,
CHALK_COBBLE,

BASALT_PEBBLE,
OBSIDIAN_PEBBLE,
GRANITE_PEBBLE,
SANDSTONE_PEBBLE,
CHALK_PEBBLE,

GRASS_FIBER,
PARSNIP_SEEDS,
APPLE,
X,
Y,

BRONZE_ORE,
SILVER_ORE,
GOLD_ORE,
Z,
W,

BRONZE_INGOT,
SILVER_INGOT,
GOLD_INGOT)

ENUM(ItemAmount,
ALL,
HALF,
ONE)
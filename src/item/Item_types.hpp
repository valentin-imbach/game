
#pragma once
#include "enum.hpp"

#define MAX_STACK 99u

ENUM(ItemKind,


//Tools
PICK_AXE,
AXE,
SHOVEL,
HAMMER,
SCYTHE,
HOE,
KNIFE,
ACCESSORY,

//Weapons
SWORD,
BOW,
ARROW,
SHIELD,





//Materials
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

//Crafting
METAL_BAR,
PLATE,
GLASS,
PAPER,
GEAR,
FABRIC,
FUEL,
POTION,

//Clothing
CLOTHING_HEAD,
CLOTHING_BODY,
CLOTHING_LEGS,
CLOTHING_FEET,

//Armor
ARMOR_HEAD,
ARMOR_BODY,
ARMOR_LEGS,
ARMOR_FEET)

ENUM(ItemProperty,
//Mechanical
WEIGHT,
STRENGTH,
HARDNESS,
FLEXIBILITY,
TOUGHNESS,

//Food
NUTRITION,

//Optical
REFLECTIVITY,
LUMINOCITY,
OPACITY,

//Fuel
BURN_TIME,
HEAT,

//Tools
EFFICIENCY,
DURABILITY,
PRECISION,
EFFECTIVENESS,
LEVEL,

//Weapons
DAMAGE,
PARRY,

//Clothing
VANITY,
COMFORT,
WARMTH)

ENUM(ItemId,
SWORD,
PICK_AXE,
AXE,
SHOVEL,
BOW,
ARROW,

BOOTS,
ROBE,
CAP,
SHIELD,
POTION,
APPLE,

PINE_LOG,
OAK_LOG,
ROSEWOOD_LOG,
ASHWOOD_LOG,
SPRUCE_LOG,
X_LOG,

PINE_PLANK,
OAK_PLANK,
ROSEWOOD_PLANK,
ASHWOOD_PLANK,
SPRUCE_PLANK,
X_PLANK,

PINE_STICK,
OAK_STICK,
ROSEWOOD_STICK,
ASHWOOD_STICK,
SPRUCE_STICK,
X_STICK,

BASALT_COBBLE,
OBSIDIAN_COBBLE,
GRANITE_COBBLE,
SANDSTONE_COBBLE,
CHALK_COBBLE,
Y_COBBLE,

BASALT_PEBBLE,
OBSIDIAN_PEBBLE,
GRANITE_PEBBLE,
SANDSTONE_PEBBLE,
CHALK_PEBBLE,
Y_PEBBLE,

GRASS_FIBER)

ENUM(ItemAmount,
ALL,
HALF,
ONE)
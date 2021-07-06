//
//  item.h
//  Game
//
//  Created by Valentin Imbach on 25.06.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

enum class ItemType {
    NONE = -1,
    
    //Raw Materials
    COBBLE,
    PEBBLE,
    CLAY,
    LOG,
    ORE,
    GEM,
    LEATHER,
    ORGANIC,
    DUST,
    
    //Crafting
    METAL_BAR,
    GLASS,
    PAPER,
    FABRIC,
    PLATE,
    ROD,
    GEAR,
    ROPE,

    //Tools
    PICK_AXE,
    AXE,
    SHOVEL,
    HAMMER,
    SCYTHE,
    HOE,
    KNIFE,
    
    X,
    
    //Weapons
    SWORD,
    BOW,
    ARROW,
    SHIELD,
    
    //Clothing
    HEAD,
    BODY,
    LEG,
    FEET,
    
    //Misc
    FOOD,
    FUEL,
    POTION,
    ACCESSORY,

    MAX
};

enum class ItemProperty {
    NONE = -1,
    
    //Mechanical
    WEIGHT,
    STENGTH,
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
    
    //Weapons
    DAMAGE,
    PARRY,
    
    //Clothing
    VANITY,
    COMFORT,
    WARMTH,
    
    MAX
};

enum class ItemID {
    
    NONE = -1,
    
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
    
    GRANITE_COBBLE,
    BASALT_COBBLE,
    LIMESTONE_COBBLE,
    SANDSTONE_COBBLE,
    CHALK_COBBLE,
    Y_COBBLE,
    
    GRANITE_PEBBLE,
    BASALT_PEBBLE,
    LIMESTONE_PEBBLE,
    SANDSTONE_PEBBLE,
    CHALK_PEBBLE,
    Y_PEBBLE,
    
    MAX
};


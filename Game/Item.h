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
    STONE,
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
    SHOVEL,
    PICK,
    HAMMER,
    SWORD,
    BOW,
    ARROW,
    STONE,
    WOOD,
    BRANCH,
    RING,
    BOOK,
    SHIELD,
    SHOES,
    ROBE,
    HELMET,
    MEAT,
    POTION,
    APPLE,
    
    MAX
};


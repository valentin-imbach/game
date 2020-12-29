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
    
    //Material
    STONE,
    CLAY,
    WOOD,
    ORE,
    GEM,
    LEATHER,
    ORGANIC,
    DUST,
    
    //Crafting
    METAL,
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
    TOOL,
    
    //Weapons
    SWORD,
    BOW,
    ARROW,
    SHIELD,
    
    //CLOTHING
    HEAD_CLOTHING,
    BODY_CLOTHING,
    LEG_CLOTHING,
    FOOT_CLOTHING,
    
    //Misc
    FOOD,
    FUEL,
    POTION,
    ACCESSORY,

    MAX
    
};

enum class ItemProperty {
    
    //Mechanical
    WEIGHT,
    STENGTH,
    HARDNESS,
    FLEXIBILITY,
    ELASTICITY,
    STIFFNESS,
    TOUGHNESS,
    
    //Liquids
    VISCOSITY,
    
    //Organic
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
    
    //Weapons
    ATTACK,
    PARRY,
    
    //Clothing
    VANITY,
    COMFORT,
    
    MAX,
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


//
//  item.h
//  Game
//
//  Created by Valentin Imbach on 25.06.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

enum class ItemType {
    
    //Material
    STONE,
    CLAY,
    WOOD,
    DUST,
    LEATHER,
    ORGANIC,
    FOOD,
    ORE,
    METAL,
    GEM,
    GLASS,
    PAPER,
    FUEL,
    SLIME,
    
    //Crafting
    FABRIC,
    ROPE,
    CHAIN,
    PLATE,
    ROD,
    GEAR,

    //Tools
    SHOVEL,
    AXE,
    PICK_AXE,
    HAMMER,
    KNIFE,
    
    //Weapons
    SWORD,
    BOW,
    ARROW,
    
    //Misc
    POTION,
    ARMOUR,
    CLOTHING,
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


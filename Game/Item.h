//
//  item.h
//  Game
//
//  Created by Valentin Imbach on 25.06.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

enum class ItemType {
    STONE,
    CLAY,
    WOOD,
    DUST,
    
    LEATHER,
    ORGANIC,
    FOOD,

    TOOL,
    SWORD,
    POTION,
    CLOTHING,
    ACCESSORY,

    ORE,
    METAL,
    GEM,
    GLASS,
    PAPER,

    FABRIC,
    ROPE,
    CHAIN,
    PLATE,
    ROD,
    GEAR,
    
    FUEL,
    
    MAX
    
};

enum class ItemProperty {
    
    //Main Properties
    WEIGHT,
    STENGTH,
    HARDNESS,
    FLEXIBILITY,
    LUMINOCITY,
    
    //Organic
    NUTRITION,
    
    //Mechanical
    ELASTICITY,
    STIFFNESS,
    TOUGHNESS,
    PLASTICITY,
    DENSITY,
    VISCOSITY,
    
    //Optical
    REFLECTIVITY,
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


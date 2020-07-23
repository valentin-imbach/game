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
};

enum class ItemProperty {
    
    //Main Properties
    WEIGHT,
    STENGTH,
    DURABILITY,
    HARDNESS,
    FLEXIBILITY,
    LUMINOCITY,
    NUTRITION,
    
    MAX,
    
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
    RINF,
    BOOK,
    SHIELD,
    SHOES,
    ROBE,
    HELMET,
    MEAT,
    POTION,
    APPLE
};

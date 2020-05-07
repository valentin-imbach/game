//
//  types.h
//  Game
//
//  Created by Valentin Imbach on 07.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

enum Direction {EAST,NEAST,NORTH,NWEST,WEST,SWEST,SOUTH,SEAST};

enum class ITEM {
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
    BOOTS,
    CLOAK,
    HELMET,
    MEAT,
    POTION,
    APPLE
};

enum class RESOURCE {
    STONE,
    TREE
};

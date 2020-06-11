//
//  EntityFactory.hpp
//  Game
//
//  Created by Valentin Imbach on 11.06.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

enum class TEMPLATE {
    TREE,
    ROCK
};

class EntityFactory {
public:
    static void createEntity();
};

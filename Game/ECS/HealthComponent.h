//
//  HealthComponent.h
//  Game
//
//  Created by Valentin Imbach on 19.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "Components.h"

class HealthComponent : public Component {
public:
    int maxHealth;
    int health;
    HealthComponent() {}
    HealthComponent(Entity* entity, int max = 20) {
        maxHealth = max;
        health = max;
    }
    
    SERIALIZE(maxHealth,health);
};
